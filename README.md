# Cansat Laika
_CanSat autónomo que mide un perfil atmosférico y transmite las variables e imágenes desde una altura de máximo 1 km._

<a>
<img width="850" src="https://github.com/DaniSTexe/Telemetry/blob/main/sources/layca_image.jpg">
</a>


# Implementation
Un Cansat esencialmente es un satélite empacado en una estructura pequeña, como la de una lata de gaseosa. Este Cansat fue realizado como proyecto final para la asignatura de instrumentación electrónica del programa de ingeniería electrónica de la Universidad Industrial de Santander. Para diseñar e implementar este proyecto se contó con un equipo de 27 estudiantes, estos recursos humanos fueron distribuidos en 7 subsistemas diferentes con el propósito de distribuir el trabajo de los diferentes requerimientos técnicos del satélite. 

Los subsistemas del Cansat son: 
1- Diseño estructural
2- Sistema de potencia
3- Microcontrolador/PCB
4- Instrumentación
5- Sistema de despliegue/paracaídas
6- Sistema de comunicaciones
7- Estación de telemetría.

El diseño responde a las necesidades planteadas a partir de la dinámica de funcionamiento establecida: El Cansat debe volar a una altura máxima de 100 metros colgando de una estructura (diseñada también en el proyecto) que va amarrada a una gondola que a su vez va colgando de un dron, una vez alcance esta altura, el cansat debe desacoplarse y caer de manera segura mientras graba el descenso desde su parte superior para obtener visión del dron mientras cae.

Este repositorio contiene el codigo utilizado para implementar el proyecto. En la carpeta ''Transmisor'' se encuentra el codigo que va dentro del microcontrolador que va en el Cansat, en este, se obtienen los datos provenientes de los sensores que van en el satelite y se transmiten a través de un radio enlace al receptor, adicionalmente, contiene la rutina a ejecutar a partir de una determinada condición de altura para poder iniciar el despliegue del Cansat. En la carpeta ''Receptor'' Se encuentra el codigo utilizado en la estación terrena para recibir los datos provenientes del radio enlace ya mencionado. En la carpeta ''Telemetry'' se encuentra el codigo utilizado para crear la interfaz de software que daba la visualización de los datos de la estación terrena.

Tanto receptor como transmisor están programados en Arduino. La interfaz de software de la estación terrena fue realizada en Python.

## Documentation 📖

* [Document](https://github.com/DaniSTexe/laika/blob/main/Documento.pdf)
