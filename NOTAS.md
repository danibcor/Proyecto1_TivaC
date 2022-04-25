# BLOC DE NOTAS

DUDA:
- ¿Usamos siempre antirrebote?


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

<br/>
<br/>

