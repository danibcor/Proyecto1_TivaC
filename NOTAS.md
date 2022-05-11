# BLOC DE NOTAS

DUDA:
- ON/OFF BMI: ¿solo timer y grafica o tambien ponemos en suspension el propio sensor?

BUGS:
- Cambiar nombre graficas acc y gyro.


CAMBIOS 25/04
<br/>1.- En 'serialprotocol.h' (CCS) se ha cambiado el tamaño del paquete mandado (de 32*2 a 100*2). Esto se hace porque estamos mandando un array de 64 posiciones de uint16_t,
lo que serían (16*64) 1024 bits, es decir, 128 bytes. Como teníamos configuradas 32*2=64 bytes no podemos enviar la trama, por tanto, aumentamos el tamaño de la misma.
<br/>NOTA: No se debe poner exactamente 128 bytes pues hya que contar con encabezado, cola, checksum, etc.
<br/>NOTA2: En Qt no hemos cambiado nada.
<br/>2.- En 'remotelink.c' (CCS) se ha añadido un MUTEX para evitar errores de stuffing entre otros. Esto es porque si varias funciones envían a la vez la trama se puede
corromper y, por tanto, fallar.

      *** FUNCIONA DPM ***
      
<br/>3.- FUNCIONALIDADES DE AUDIO EN QT. Hay que añadir:
<br/>     'mainuserui.h': #include QMediDevices, QAudioSink y QIODevice.
<br/>     Además en private: QMediaDevices *m_devices = nullptr; QAudioSink *m_audioOutput=nullptr;
<br/>     QIODevice *m_device=nullptr;
<br/>     'XXX.pro': añadir 'multimedia' en el 'QT      +='

CAMBIOS 26/04
<br/>1.- Se ha hecho todo hasta 2.3 y parte de las especificaciones del punto 4.
<br/>2.- Control de errores: ya no se puede activar el 'PLAY' si la frecuencia es menor a 4000.
<br/>3.- Control de versiones ----> VERSION 7
<br/>4.- Comenzamos a hacer el ultimo punto: 2.4.1

