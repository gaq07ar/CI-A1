# Proyecto de cerradura inteligente utilizando Arduino Mega 2560 y módulo de RFID
## Descripción 

El proyecto consiste en la implementación de una cerradura electrónica activada mediante una etiqueta o tag RFID.
Dicha cerradura inteligente permitirá que un usuario con un llavero admitido pueda ingresar con el solo hecho de apoyar el llavero sobre el lector y solo usuarios admitidos puedan entrar. De esta forma podremos asignar más de un usuario a una misma cerradura (cada cerradura contará con una lista de llaveros admitidos). Y podremos asignar la misma llave a diferentes cerraduras. Por ejemplo, un profesor podría tener acceso al laboratorio y al aula 42 si así se determina que tiene que ser.
Si la clave es correcta se habilita el acceso mostrando un mensaje de bienvenida y se activa el mecanismo de apertura, en caso contrario se lanza el mensaje error.

## Materiales mecesarios

- Arduino Mega 2560 (Rev3)
- LCD Keypad Shield.
- Driver Motor Stepper (ULN2003A).
- Arduino Ethernet Shield (Rev3)
- 28BYJ-48 Stepper Motor
- RFID-RC522-v3.
- YwRobot Breadboard Power Supply.
- Cables.

Whenever you commit to this repository, GitHub Pages will run [Jekyll](https://jekyllrb.com/) to rebuild the pages in your site, from the content in your Markdown files.

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/gaq07ar/CI-A1/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.
