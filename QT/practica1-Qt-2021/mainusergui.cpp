#include "mainusergui.h"
#include "ui_mainusergui.h"
#include <QSerialPort>      // Comunicacion por el puerto serie
#include <QSerialPortInfo>  // Comunicacion por el puerto serie
#include <QMessageBox>      // Se deben incluir cabeceras a los componentes que se vayan a crear en la clase
// y que no estén incluidos en el interfaz gráfico. En este caso, la ventana de PopUp <QMessageBox>
// que se muestra al recibir un PING de respuesta

#include<stdint.h>      // Cabecera para usar tipos de enteros con tamaño
#include<stdbool.h>     // Cabecera para usar booleanos

MainUserGUI::MainUserGUI(QWidget *parent) :  // Constructor de la clase
    QWidget(parent),
    ui(new Ui::MainUserGUI)               // Indica que guipanel.ui es el interfaz grafico de la clase
  , transactionCount(0)
{
    ui->setupUi(this);                // Conecta la clase con su interfaz gráfico.
    setWindowTitle(tr("Interfaz de Control TIVA")); // Título de la ventana

    // Inicializa la lista de puertos serie
    ui->serialPortComboBox->clear(); // Vacía de componentes la comboBox
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        // La descripción realiza un FILTRADO que  nos permite que SOLO aparezcan los interfaces tipo USB serial con el identificador de fabricante
        // y producto de la TIVA
        // NOTA!!: SI QUIERES REUTILIZAR ESTE CODIGO PARA OTRO CONVERSOR USB-Serie, cambia los valores VENDOR y PRODUCT por los correspondientes
        // o cambia la condicion por "if (true) para listar todos los puertos serie"
        if ((info.vendorIdentifier()==0x1CBE) && (info.productIdentifier()==0x0002))
        {
            ui->serialPortComboBox->addItem(info.portName());
        }
    }



    /*
     * ESPECIFICACION 4.3 PARA EL CONTROL DE AUDIO
     */

    //Este objeto es necesario para poder gestionar los dispositivos multimedia.
    m_devices = new QMediaDevices(this);

    // Inicializo combobox con los dispositivos de salida de audio.
    const QAudioDevice &defaultDeviceInfo = m_devices->defaultAudioOutput();
    ui->AudioDevices->addItem(defaultDeviceInfo.description(), QVariant::fromValue(defaultDeviceInfo));
    for(auto &deviceInfo: m_devices->audioOutputs())
    {
        if(deviceInfo != defaultDeviceInfo)
        {
            ui->AudioDevices->addItem(deviceInfo.description(), QVariant::fromValue(deviceInfo));
        }
    }

    // Deshabilitar el boton de reproducir audio al iniciar el programa (ADC_CKECK estará desactivado)
    ui->PLAY->setDisabled(true);


    /*
     * ESPECIFICACION 4.2 PARA GRAFICAR
     */

    // Inicializo Grafica 1
    ui->graficaPB5->setTitle("ADC PB5");;
    ui->graficaPB5->setAxisTitle(QwtPlot::xBottom, "Tiempo");
    ui->graficaPB5->setAxisTitle(QwtPlot::yLeft, "Voltaje");
    ui->graficaPB5->setAxisScale(QwtPlot::yLeft, 0, 3.5);
    ui->graficaPB5->setAxisScale(QwtPlot::xBottom,0, 4096.0);

    // Formato de la curva
    curva = new QwtPlotCurve();
    curva->attach(ui->graficaPB5);

    for(int i = 0;i < 4096; i++)
    {
        xVal[i] = i;
        yVal[i] = 0;
    }

    // Inicializa los datos apuntando a bloques de memoria, por eficiencia
    curva->setRawSamples(xVal, yVal, 4096);

    // Color de la curva
    curva->setPen(QPen(Qt::red));

    // Rejilla de puntos
    m_Grid = new QwtPlotGrid();
    // que se asocia al objeto QwtPl
    m_Grid->attach(ui->graficaPB5);
    //Desactiva el autoreplot (mejora la eficiencia)
    ui->graficaPB5->setAutoReplot(false);

    //Inicializa algunos controles
    ui->serialPortComboBox->setFocus();   // Componente del GUI seleccionado de inicio
    ui->pingButton->setEnabled(false);    // Se deshabilita el botón de ping del interfaz gráfico, hasta que

    //Inicializa la ventana de PopUp que se muestra cuando llega la respuesta al PING
    ventanaPopUp.setIcon(QMessageBox::Information);
    ventanaPopUp.setText(tr("Status: RESPUESTA A PING RECIBIDA")); //Este es el texto que muestra la ventana
    ventanaPopUp.setStandardButtons(QMessageBox::Ok);
    ventanaPopUp.setWindowTitle(tr("Evento"));
    ventanaPopUp.setParent(this,Qt::Popup);

    //Conexion de signals de los widgets del interfaz con slots propios de este objeto
    connect(ui->rojo,SIGNAL(toggled(bool)),this,SLOT(cambiaLEDs()));
    connect(ui->verde,SIGNAL(toggled(bool)),this,SLOT(cambiaLEDs()));
    connect(ui->azul,SIGNAL(toggled(bool)),this,SLOT(cambiaLEDs()));

    // Conexion de los modos
    connect(ui->PWM_GPIO,SIGNAL(toggled(bool)),this,SLOT(cambiaMODO()));

    // Conexion RGB
    connect(ui->colorWheel,SIGNAL(colorSelected(QColor)),this,SLOT(colorWheel_cambiaRGB(QColor)));

    // Conexion Rueda
    connect(ui->Knob,SIGNAL(valueChanged(double)),this,SLOT(cambiaBrillo(double)));

    // Conexion boton de estado switches
    connect(ui->botonEstado,SIGNAL(clicked()),this,SLOT(comprobarEstado()));

    // Conexion boton de estado switches por eventos
    connect(ui->botonEstado_evento,SIGNAL(clicked()),this,SLOT(comprobarEstado_Eventos()));

    // Conexion boton activar tomas de datos frecuencias de muestreo
    connect(ui->ADCcheck,SIGNAL(clicked()),this,SLOT(on_ADCcheck_clicked()));

    // Conexion cambio de las frecuencias de muestreo
    connect(ui->boton_frec,SIGNAL(valueChanged(double)),this,SLOT(on_boton_frec_valueChanged(double)));

    //Conectamos Slots del objeto "Tiva" con Slots de nuestra aplicacion (o con widgets)
    connect(&tiva,SIGNAL(statusChanged(int,QString)),this,SLOT(tivaStatusChanged(int,QString)));
    connect(&tiva,SIGNAL(messageReceived(uint8_t,QByteArray)),this,SLOT(messageReceived(uint8_t,QByteArray)));
}

MainUserGUI::~MainUserGUI() // Destructor de la clase
{
    delete ui;   // Borra el interfaz gráfico asociado a la clase
}

void MainUserGUI::on_serialPortComboBox_currentIndexChanged(const QString &arg1)
{
    activateRunButton();
}

// Funcion auxiliar de procesamiento de errores de comunicación
void MainUserGUI::processError(const QString &s)
{
    activateRunButton(); // Activa el botón RUN
    // Muestra en la etiqueta de estado la razón del error (notese la forma de pasar argumentos a la cadena de texto)
    ui->statusLabel->setText(tr("Status: Not running, %1.").arg(s));
}

// Funcion de habilitacion del boton de inicio/conexion
void MainUserGUI::activateRunButton()
{
    ui->runButton->setEnabled(true);
}


// SLOT asociada a pulsación del botón RUN
void MainUserGUI::on_runButton_clicked()
{
    //Intenta arrancar la comunicacion con la TIVA
    tiva.startRemoteLink( ui->serialPortComboBox->currentText());
}

//Slots Asociado al boton que limpia los mensajes del interfaz
void MainUserGUI::on_pushButton_clicked()
{
    ui->statusLabel->setText(tr(""));
}

void MainUserGUI::cambiaBrillo(double value)
{

    MESSAGE_LED_PWM_BRIGHTNESS_PARAMETER brillo;

    brillo.rIntensity = 250 * value;

    tiva.sendMessage(MESSAGE_LED_PWM_BRIGHTNESS,QByteArray::fromRawData((char *)&brillo,sizeof(brillo)));
}

void MainUserGUI::on_ADCButton_clicked()
{
    tiva.sendMessage(MESSAGE_ADC_SAMPLE,NULL,0);
}

void MainUserGUI::on_pingButton_clicked()
{
    tiva.sendMessage(MESSAGE_PING,NULL,0);
}

void MainUserGUI::cambiaLEDs(void)
{

    MESSAGE_LED_GPIO_PARAMETER parameter;

    parameter.leds.red=ui->rojo->isChecked();
    parameter.leds.green=ui->verde->isChecked();
    parameter.leds.blue=ui->azul->isChecked();

    tiva.sendMessage(MESSAGE_LED_GPIO,QByteArray::fromRawData((char *)&parameter,sizeof(parameter)));
}

//**** Slot asociado a la recepción de mensajes desde la TIVA ********/
//Está conectado a una señale generada por el objeto TIVA de clase QTivaRPC (se conecta en el constructor de la ventana, más arriba en este fichero))
//Se pueden añadir los que casos que quieran para completar la aplicación

void MainUserGUI::messageReceived(uint8_t message_type, QByteArray datos)
{
    switch(message_type) // Segun el comando tengo que hacer cosas distintas
    {
        /** A PARTIR AQUI ES DONDE SE DEBEN AÑADIR NUEVAS RESPUESTAS ANTE LOS COMANDOS QUE SE ENVIEN DESDE LA TIVA **/
        case MESSAGE_PING:
        {   //Recepcion de la respuesta al ping desde la TIVA
            // Algunos comandos no tiene parametros --> No se extraen
            ventanaPopUp.setStyleSheet("background-color: lightgrey");
            ventanaPopUp.setModal(true); //CAMBIO: Se sustituye la llamada a exec(...) por estas dos.
            ventanaPopUp.show();
        }
        break;

        case MESSAGE_REJECTED:
        {   //Recepcion del mensaje MESSAGE_REJECTED (La tiva ha rechazado un mensaje que le enviamos previamente)
            MESSAGE_REJECTED_PARAMETER parametro;
            if (check_and_extract_command_param(datos.data(), datos.size(), &parametro, sizeof(parametro))>0)
            {
                // Muestra en una etiqueta (statuslabel) del GUI el mensaje
            ui->statusLabel->setText(tr("Status: Comando rechazado,%1").arg(parametro.command));
            }
            else
            {
                ui->statusLabel->setText(tr("Status: MSG %1, recibí %2 B y esperaba %3").arg(message_type).arg(datos.size()).arg(sizeof(parametro)));
            }
        }
        break;

        case MESSAGE_ADC_SAMPLE:
        {    // Este caso trata la recepcion de datos del ADC desde la TIVA
            MESSAGE_ADC_SAMPLE_PARAMETER parametro;
            if (check_and_extract_command_param(datos.data(), datos.size(), &parametro, sizeof(parametro))>0)
            {
                // La cuenta que se hace es para pasarlo a voltios, se multiplica por VCC de 3.3
                // y se divido entre 4096 porque es un convertidor de 12 bits
                ui->lcdCh1->display(((double)parametro.chan1)*3.3/4096.0);
                ui->lcdCh2->display(((double)parametro.chan2)*3.3/4096.0);
                ui->lcdCh3->display(((double)parametro.chan3)*3.3/4096.0);
                ui->lcdCh4->display(((double)parametro.chan4)*3.3/4096.0);
                ui->lcdCh5->display(((double)parametro.chan5)*3.3/4096.0);
                ui->lcdCh6->display(((double)parametro.chan6)*3.3/4096.0);
            }
            else
            {   //Si el tamanho de los datos no es correcto emito la senhal statusChanged(...) para reportar un error
                ui->statusLabel->setText(tr("Status: MSG %1, recibí %2 B y esperaba %3").arg(message_type).arg(datos.size()).arg(sizeof(parametro)));
            }

        }
        break;

        case MESSAGE_64_MUESTRAS:
        {    // Este caso trata la recepcion de datos del ADC desde la TIVA
            MESSAGE_64_MUESTRAS_PARAMETER parametros;
            double valor_frec = ui->boton_frec->value();

            if (check_and_extract_command_param(datos.data(), datos.size(), &parametros, sizeof(parametros)) > 0)
            {
                // La cuenta que se hace es para pasarlo a voltios, se multiplica por VCC de 3.3
                // y se divido entre 4096 porque es un convertidor de 12 bits
                ui->lcd_micro->display(((double)parametros.valor[0])*3.3/4096.0);

                int i;

                for(i = 0;i < 4096-64;i++)
                {
                    yVal[i] = yVal[i + 64];
                }
                for(i = 0;i < 64;i++)
                {
                    yVal[4032 + i] = ((double)parametros.valor[i]*3.3)/4096.0;
                }
                ui->graficaPB5->replot();


                if(ui->PLAY->isChecked())
                {
                    int j;

                    for(j = 0;j < 3;j++)
                    {
                        for(i = 0;i < 64; i++)
                        {
                            sonido[(64 * contador) + i] = parametros.valor[i];
                        }

                        contador++;
                    }

                    if(contador == 9)
                    {
                        m_device->write((char *)sonido, sizeof(uint16_t) * MAX_SAMPLES_SOUND);
                        contador = 0;
                    }
                }
            }
            else
            {   //Si el tamanho de los datos no es correcto emito la senhal statusChanged(...) para reportar un error
                ui->statusLabel->setText(tr("Error al recibir 64 muestras"));
            }

        }

        break;

        case MESSAGE_ESTADO_SWITCH:
        {
            MESSAGE_ESTADO_SWITCH_PARAMETER estado;

            if (check_and_extract_command_param(datos.data(), datos.size(), &estado, sizeof(estado)) > 0)
            {
                // Switch 1 y Switch 2 pulsados, encendemos los 2 leds
                if(estado.switch1 == 0 && estado.switch2 == 0)
                {
                    ui->led_rojo->setChecked(1);
                    ui->led_azul->setChecked(1);
                }
                else if(estado.switch1 == 0)
                // Switch 1 pulsado y el Switch 2 no, enciendo el led rojo
                {
                    ui->led_rojo->setChecked(1);
                    ui->led_azul->setChecked(0);
                }
                // Switch 2 pulsado y el Switch 1 no, enciendo el led azul
                else if(estado.switch2 == 0)
                {
                    ui->led_azul->setChecked(1);
                    ui->led_rojo->setChecked(0);
                }
                // Switch 2 o Switch 1 no pulsado, apago los 2 leds
                else
                {
                    ui->led_rojo->setChecked(0);
                    ui->led_azul->setChecked(0);
                }
            }
            else
            {   //Si el tamanho de los datos no es correcto emito la senhal statusChanged(...) para reportar un error
                ui->statusLabel->setText(tr("Error al recibir el estado de los switches"));
            }

        }
        break;

        case MESSAGE_ESTADO_SWITCH_EVENTOS:
        {
            MESSAGE_ESTADO_SWITCH_EVENTOS_PARAMETER estado;

            if (check_and_extract_command_param(datos.data(), datos.size(), &estado, sizeof(estado)) > 0)
            {
                // Switch 1 y Switch 2 pulsados, encendemos los 2 leds
                if(estado.switch1 == 0 && estado.switch2 == 0)
                {
                    ui->led_rojo_evento->setChecked(1);
                    ui->led_azul_evento->setChecked(1);
                }
                // Switch 1 pulsado y el Switch 2 no, enciendo el led rojo
                else if(estado.switch1 == 0)
                {
                    ui->led_rojo_evento->setChecked(1);
                    ui->led_azul_evento->setChecked(0);
                }
                // Switch 2 pulsado y el Switch 1 no, enciendo el led azul
                else if(estado.switch2 == 0)
                {
                    ui->led_azul_evento->setChecked(1);
                    ui->led_rojo_evento->setChecked(0);
                }
                // Switch 2 o Switch 1 no pulsado, apago los 2 leds
                else
                {
                    ui->led_rojo_evento->setChecked(0);
                    ui->led_azul_evento->setChecked(0);
                }
            }
            else
            {   //Si el tamanho de los datos no es correcto emito la senhal statusChanged(...) para reportar un error
                ui->statusLabel->setText(tr("Error al recibir el estado de los switches por eventos"));
            }

        }
        break;

        default:
            //Notifico que ha llegado un tipo de mensaje desconocido
            ui->statusLabel->setText(tr("Status: Recibido mensaje desconocido %1").arg(message_type));
        break; //Innecesario
    }
}

//Este Slot lo hemos conectado con la señal statusChange de la TIVA, que se activa cuando
//El puerto se conecta o se desconecta y cuando se producen determinados errores.
//Esta función lo que hace es procesar dichos eventos
void MainUserGUI::tivaStatusChanged(int status,QString message)
{
    switch (status)
    {
        case TivaRemoteLink::TivaConnected:

            //Caso conectado..
            // Deshabilito el boton de conectar
            ui->runButton->setEnabled(false);

            // Se indica que se ha realizado la conexión en la etiqueta 'statusLabel'
            ui->statusLabel->setText(tr("Ejecucion, conectado al puerto %1.").arg(ui->serialPortComboBox->currentText()));

            //   Y se habilitan los controles deshabilitados
            ui->pingButton->setEnabled(true);

        break;

        case TivaRemoteLink::TivaIsDisconnected:
            //Caso desconectado..
            // Rehabilito el boton de conectar
             ui->runButton->setEnabled(false);
             ui->statusLabel->setText(message);
        break;

        case TivaRemoteLink::FragmentedPacketError:

        case TivaRemoteLink::CRCorStuffError:
            //Errores detectados en la recepcion de paquetes
            ui->statusLabel->setText(message);

        break;

        default:
            //Otros errores (por ejemplo, abriendo el puerto)
            processError(tiva.getLastErrorMessage());
    }
}

void MainUserGUI::cambiaMODO()
{
    MESSAGE_MODE_PARAMETER modo;

    if(ui->PWM_GPIO->isChecked())
    {
        modo.modotx = 1;
    }
    else
    {
        modo.modotx = 0;
    }

    tiva.sendMessage(MESSAGE_MODE,QByteArray::fromRawData((char *)&modo,sizeof(modo)));
}


void MainUserGUI::colorWheel_cambiaRGB(const QColor &arg1)
{
    MESSAGE_RGB_PARAMETER color;

    color.R = arg1.red();
    color.G = arg1.green();
    color.B = arg1.blue();

    tiva.sendMessage(MESSAGE_RGB,QByteArray::fromRawData((char *)&color,sizeof(color)));
}

// Vamos a mandar un mensaje a la Tiva para que sepa que queremos obtener el estado de los switches
// Desde la propia tiva una vez recibida el mensaje la propia tiva colocará los valores necesarios
// a la variable estado y mandará un mensaje a QT. QT tratará este mensaje y otros en la funcion
// void MainUserGUI::messageReceived(uint8_t message_type, QByteArray datos)
// esta función se encuentra mas arriba en este fichero
void MainUserGUI::comprobarEstado()
{
    MESSAGE_ESTADO_SWITCH_PARAMETER estado;
    tiva.sendMessage(MESSAGE_ESTADO_SWITCH,QByteArray::fromRawData((char *)&estado,sizeof(estado)));
}

void MainUserGUI::comprobarEstado_Eventos()
{
    MESSAGE_ESTADO_SWITCH_EVENTOS_PARAMETER estado;
    tiva.sendMessage(MESSAGE_ESTADO_SWITCH_EVENTOS,QByteArray::fromRawData((char *)&estado,sizeof(estado)));
}

// Boton para activar el cambio de la frecuencia de muestreo
void MainUserGUI::on_ADCcheck_clicked()
{
    MESSAGE_ACTIVAR_MUESTREO_PARAMETER parametro;

    if(ui->ADCcheck->isChecked())
    {
        parametro.activar = 1;
        parametro.valor = ui->boton_frec->value();

        // Habilitar el boton de reproducir audio
        ui->PLAY->setDisabled(false);
    }
    else
    {
        parametro.activar = 0;
        parametro.valor = -1;

        // Desmarcar el boton de reproducir audio, NOTA: También se entra en la funcion y desactiva el audio
        ui->PLAY->setChecked(false);
        // Deshabilitar el boton de reproducir audio
        ui->PLAY->setDisabled(true);
    }

    tiva.sendMessage(MESSAGE_ACTIVAR_MUESTREO, QByteArray::fromRawData((char *)&parametro, sizeof(parametro)));
}


// Boton para cambiar la frecuencia de muestreo
void MainUserGUI::on_boton_frec_valueChanged(double arg1)
{
    // Sólo mandaremos el mensaje de cambio de la frecuencia si el botón de activar el cambio de frecuencia se encutra activo
    if(ui->ADCcheck->isChecked())
    {
        MESSAGE_FRECUENCIA_MUESTREO_PARAMETER valor_frec;
        valor_frec.valor = arg1;
        tiva.sendMessage(MESSAGE_FRECUENCIA_MUESTREO, QByteArray::fromRawData((char *)&valor_frec, sizeof(valor_frec)));


        /*
         *     CONFIGURACION DEL DISPOSITIVO DE AUDIO AL CAMBIA DE FRECUENCIA
         */

        if(arg1 < 4000)
        {
            // Desmarcar el boton de reproducir audio, NOTA: También se entra en la funcion y desactiva el audio
            ui->PLAY->setChecked(false);
            // Deshabilitar el boton de reproducir audio
            ui->PLAY->setDisabled(true);
        }
        else
        {
            // Habilitar el boton de reproducir audio
            ui->PLAY->setDisabled(false);
        }

        if(ui->PLAY->isChecked())
        {
            const QAudioDevice &deviceInfo = ui->AudioDevices->currentData().value<QAudioDevice>();

            //Destruye el QAudioSink anterior si estaba creado
            if (m_audioOutput != nullptr)
            {
                delete m_audioOutput;
                m_audioOutput = nullptr;
            }

            //Crea un nuevo QAudioSink con los nuevos parámetros.
            QAudioFormat desiredFormat;
            desiredFormat.setChannelCount(1);
            desiredFormat.setSampleFormat(QAudioFormat::Int16);

            desiredFormat.setSampleRate(arg1); //Frecuencia de muestreo, multiplicamos por 3 para aumentar la frecuencia  y tener mínimo 12kHz

            if (!deviceInfo.isFormatSupported(desiredFormat))
            {
                ui->statusLabel->setText("AA Raw audio format not supported by backend, cannot play audio.");
                return;
             }

             m_audioOutput = new QAudioSink(deviceInfo, desiredFormat);
        }
    }
}

// Este slot se ejecuta al seleccionar un nuevo dispositivo de audio para reproducción (o al inicializar la lista de dispositivos)
void MainUserGUI::on_AudioDevices_currentIndexChanged(int index)
{
    const QAudioDevice &deviceInfo = ui->AudioDevices->currentData().value<QAudioDevice>();

    //Si ya se había inicializado el dispositivo de audio (se ha cambiado a otro) se libera el anterior
    if (m_audioOutput != nullptr)
    {
      delete m_audioOutput;
      m_audioOutput = nullptr;
    }

    // Inicializa el nuevo dispositivo de audio seleccionado y configura el formato
    QAudioFormat desiredFormat;
    desiredFormat.setChannelCount(1); //1 Canal
    desiredFormat.setSampleFormat(QAudioFormat::Int16); //Formato entero de 16bits con signo.

    desiredFormat.setSampleRate(ui->boton_frec->value()); //Frecuencia de muestreo, multiplicamos por 3 para aumentar la frecuencia  y tener mínimo 12kHz

    if(!deviceInfo.isFormatSupported(desiredFormat))
    {
      ui->statusLabel->setText("BB Raw audio format not supported by backend, cannot play audio.");
      return;
    }

    m_audioOutput = new QAudioSink(deviceInfo, desiredFormat); //Crea el QAudioSink con el formato deseado
}


void MainUserGUI::on_PLAY_toggled(bool checked)
{
    if(checked)
    {
        // Deshabilitar el cambio de frecuencia mientras se reproduce el audio
        ui->boton_frec->setDisabled(true);
        //Arranca la reproducción , obteniendo un QIODevice donde enviar las muestras ("modo PUSH de QAudioSink")
        m_device = m_audioOutput->start();
    }
    else
    {
        // Habilitar el cambio de frecuencia tras dejar de reproducir el audio
        ui->boton_frec->setDisabled(false);
        //Detiene la reproducción de Audio.
        m_audioOutput->stop();
    }
}

