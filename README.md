# Proyecto de cerradura inteligente utilizando Arduino Mega 2560 y módulo de RFID

## Descripción 

El proyecto consiste en la implementación de una cerradura electrónica activada mediante una etiqueta o tag RFID.
Dicha cerradura inteligente permitirá que un usuario con un llavero admitido pueda ingresar con el solo hecho de apoyar el llavero sobre el lector y solo usuarios admitidos puedan entrar. De esta forma podremos asignar más de un usuario a una misma cerradura (cada cerradura contará con una lista de llaveros admitidos). Y podremos asignar la misma llave a diferentes cerraduras. Por ejemplo, un profesor podría tener acceso al laboratorio y al aula 42 si así se determina que tiene que ser.
Si la clave es correcta se habilita el acceso mostrando un mensaje de bienvenida y se activa el mecanismo de apertura, en caso contrario se lanza el mensaje error.

## Materiales necesarios

- Arduino Mega 2560 (Rev3)
- LCD Keypad Shield.
- Driver Motor Stepper (ULN2003A).
- Arduino Ethernet Shield (Rev3)
- 28BYJ-48 Stepper Motor
- RFID-RC522-v3.
- YwRobot Breadboard Power Supply.
- Cables.
