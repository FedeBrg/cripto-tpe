# cripto-tpe
##Make file
Se utiliza un make file para facilitar el proceso de correr y compilar el programa y sus diferentes modulos. Los comandos aceptados son:

- make all: compila todos los aspectos del trabajo practico en un ejecutable llamado app
- make clean: borra todos los archivos intermedios y el ejecutable

##Ejecucion del programa
Luego de correr el comando make all, se genera un archivo llamado app en la carpeta actual. Los parametros del programa son los siguientes:

./app [tipo] [imagenSecreta] [número (k)] [directorio]

Donde tipo puede ser:
- d: distribuir una imagen 
- r: recuperar una imagen

imagenSecreta: Corresponde al nombre de un archivo de extensión .bmp. En el caso de que
se haya elegido la opción (d) este archivo debe existir ya que es la imagen a ocultar y debe ser
una imagen en blanco y negro (8 bits por pixel) Si se eligió la opción (r) este archivo será el
archivo de salida, con la imagen secreta revelada al finalizar el programa.

Número k: El número corresponde a la cantidad mínima de sombras necesarias para
recuperar el secreto en un esquema (k, n).

directorio: El directorio donde se encuentran las imágenes en las que se distribuirá el
secreto (en el caso de que se haya elegido la opción (d)), o donde están las imágenes que
contienen oculto el secreto ( en el caso de que se haya elegido la opción (r)). Debe contener
imágenes de extensión .bmp, de 8 bits por pixel, de igual tamaño que la imagen secreta.
Además, deberá verificarse que existan por lo menos k imágenes en el directorio.

Ejemplo:
Distribuir la imagen “Albert.bmp” según esquema (4,8) guardando las sombras en
imágenes del directorio “test_bmps”:

./app d Albert.bmp 4 test_bmps/

