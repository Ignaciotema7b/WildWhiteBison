# GameTemplate

Plantilla base para la creación de proyectos de juegos (C++) en Visual Studio.

Este repositorio está diseñado para acelerar la creación de nuevos proyectos, configurando automáticamente:
- Nombre del proyecto
- Archivos principales
- Estructura base
  
## ¿Cómo usar esta plantilla?

### 1. Crear un repositorio desde la plantilla:   
  Ir a este repositorio: ```GameTemplate```.
  Click en "Use this template".
  Crear un nuevo repositorio (ej: MiNuevoJuego).

### 2. Clonar el proyecto en Visual Studio
En Visual Studio:
Abrir Visual Studio.
Ir a:
Archivo → Clonar repositorio.
Pegar la URL del repo que acabás de crear
(ej: https://github.com/tu-usuario/MiNuevoJuego).
Elegir la carpeta local.
Click en Clonar.

### 3. Resolver configuración inicial
Al abrir el proyecto, puede aparecer un mensaje tipo:
MSVC v143 (o similar) no encontrado.
En ese caso:
  Click en "Omitir y aplicar".

## Notas:
- El proceso de inicialización ocurre automáticamente en el primer push
- Al inicializar, todo el texto que coincide con 'GameTemplate' es reemplazado por el nombre del repositorio (incluye archivos, clases, carpetas, etc.)
- Por lo que se recomienda usar nombres de repositorio sin espacios ni caracteres especiales
- No es necesario ejecutar scripts manuales
