## Notas de la tarea
---

### Listado de herramientas 🧰

- Uso de WSL y Windows (Local) 
    - WSL: Ubuntu v22.05 LTS.
    - Windows 11 Pro v22H2.
- Uso de Linux (Remoto en Azure Virutal Machines)
    - Linux: Ubuntu v22.05.3 LTS. 
    - Acceso por medio de SSH desde WSL.
- Docker:
    - Docker Desktop v4.22.1 en Windows 11.
    - Accesado por WSL.
- Código en C:
    - Programado, compilado y ejecutado en WSL y Linux.
    - Compilador GCC (v11,4.0 for Ubuntu.)en WSL y Linux.
    - Uso de Makefile como herramientas de compilación automatizada.
    - Librerías
- Código en Bash:
    - Escritura de archivos .sh para la automatización de procesos.

---

### Comandos 🖥️

#### Process Running

https://stackabuse.com/bytes/how-to-kill-a-process-using-a-port-in-linux/

    $ lsof -i :1717 -t


#### Run Docker Locallly

**Running only client in local**
    $ docker build -t client-app .
    $ docker run -it --network host client-app

**Running only server in local**
    $ docker build -t server-app .
    $ docker run -it -p 1717:1717 server-app

**Running both in local**
    $ docker network create mynetwork
    $ docker run -it -p 1717:1717 --network mynetwork servidor-app
    $ docker run -it --network mynetwork cliente-app

**Running client in local and server in remote VM**
    $ docker build -t client-app-msg .
    $ docker run -it client-app ./client 20.163.149.207 1717

#### Azure VM

**Access Locally the Remote VM**

    $ ssh -i Laboratory-1_key.pem ranmsy@20.163.149.207

**Copy File**

    $ scp -i Laboratory-1_key.pem /home/ranmsy/GitHub/CE4303-Tarea-2/read-write-example/server.c ranmsy@20.163.149.207:/home/ranmsy/labs


---
### -lpthread 🅿️


El uso de -lpthread en el comando de compilación se debe a la inclusión de la biblioteca de subprocesos (pthread) en el código del servidor. Aquí hay una explicación más detallada:

pthread (POSIX Threads): pthread es una biblioteca en sistemas Unix y Unix-like que proporciona un conjunto de funciones para la creación, gestión y sincronización de subprocesos. Los subprocesos permiten que un programa realice múltiples tareas en paralelo. En el contexto del servidor, esto es útil para manejar múltiples conexiones de clientes simultáneamente, lo que es común en las aplicaciones de red.

Servidor Multihilo: En el código del servidor, se utiliza pthread para crear y gestionar hilos separados para manejar las conexiones de los clientes. Cada vez que un cliente se conecta, se crea un nuevo hilo para atender esa conexión de manera concurrente sin bloquear el servidor principal.

Compilación con -lpthread: Para que el código que utiliza pthread compile correctamente, es necesario incluir la opción -lpthread durante el proceso de compilación. Esto enlaza el programa con la biblioteca de subprocesos, lo que permite que las funciones pthread se utilicen correctamente en el código.

En resumen, la inclusión de -lpthread es esencial para garantizar que el servidor pueda manejar múltiples conexiones de clientes de manera concurrente utilizando subprocesos. Sin esta opción, el código del servidor que utiliza pthread no se compilaría correctamente y no podría aprovechar la concurrencia para atender a varios clientes al mismo tiempo.