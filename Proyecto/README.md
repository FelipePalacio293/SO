[![N|Universidad](https://www2.javerianacali.edu.co/sites/ujc/files/field/image/puj_logo_azul_copia1.png)](https://www2.javerianacali.edu.co/sites/ujc/files/field/image/puj_logo_azul_copia1.png)
# Unix shell usando procesos.
_Por Felipe Palacio y Santiago Grisales_
## Descripción del proyecto.
Diseñar un programa en C que sirva como interfaz de Shell para aceptar los comandos del usuario y luego ejecutar cada uno de ellos durante su proceso. Su implementación admitirá la redirección de entradas y salidas mediante el uso de Pipes entre comandos, también admitirá el uso del comando !! para consultar la historia.

- Crear un proceso hijo y ejecutar el comando en este.
- Proveer un historial de los comandos y sus salidas.
- Añadir soporte de redirección de entradas y salidas.
- Utilizar una cadena de procesos conectados (Pipe).

> Nota: Recordar el uso de algunos systemcall tales como fork(), exec(), wait(), dup2(), and pipe().

## Manual de usuario.
Se crea y configura un makefile para ejecutar todos los archivos.

```sh
$ make
```

Enlace de Github: [Github](https://github.com/FelipePalacio293/SO/tree/master)
