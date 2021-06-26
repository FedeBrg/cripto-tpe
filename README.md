# Secreto Compartido con Esteganografía
## Introducción
Este Trabajo Práctico fue realizado para la materia Criptografía y Seguridad del Instituto Tecnológico de Buenos Aires.
## Autores
Este trabajo pertenece al Grupo N°3:
- Baader, Juan Martin
- Bergagna, Federico Manuel
- Rodríguez Brizi, Manuel
## Make file
Se utiliza un make file para facilitar el proceso de correr y compilar el programa y sus diferentes modulos. Los comandos aceptados son:

- `make all`: compila todos los archivos del trabajo practico en un ejecutable llamado ss
- `make clean`: borra todos los archivos intermedios y el ejecutable

## Ejecución del programa
Luego de correr el comando make all, se genera un archivo llamado ss en la carpeta actual. Los parámetros del programa son los siguientes:

`$> ./ss [tipo] [imagenSecreta] [k] [directorio]`

Donde `tipo` puede ser:
- d: distribuir una imagen 
- r: recuperar una imagen

`imagenSecreta`: Corresponde al nombre de un archivo de extensión .bmp. En el caso de que
se haya elegido la opción (d) este archivo debe existir ya que es la imagen a ocultar y debe ser
una imagen en blanco y negro (8 bits por pixel) Si se eligió la opción (r) este archivo será el
archivo de salida, con la imagen secreta revelada al finalizar el programa.

`k`: El número corresponde a la cantidad mínima de sombras necesarias para
recuperar el secreto en un esquema (k, n).

`directorio`: El directorio donde se encuentran las imágenes en las que se distribuirá el
secreto (en el caso de que se haya elegido la opción (d)), o donde están las imágenes que
contienen oculto el secreto ( en el caso de que se haya elegido la opción (r)). Debe contener
imágenes de extensión .bmp, de 8 bits por pixel, de igual tamaño que la imagen secreta.
Además, deberá verificarse que existan por lo menos k imágenes en el directorio.

## Ejemplos
Para distribuir la imagen “Albert.bmp” según esquema (4,8) tomando las imagenes del 
directorio “test_bmps”:

`$> ./ss d Albert.bmp 4 test_bmps/`

Esto crea el directorio "portadoras_generadas" donde se guardan las imagenes con las
sombras distribuidas.

Para recuperar la imagen preivamente distribuida en un esquema (4,8) buscando imágenes en el
directorio “portadoras_generadas” y guardandola en "secreto.bmp"

`$> ./ss r secreto.bmp 4 portadoras_generadas/`


