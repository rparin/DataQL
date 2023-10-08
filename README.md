<div align="center">

# DataQL

[![License][license.io]][license-url]

<p align="left">
A C++ program that parses user SQL-like commands through a state machine. It then determines the type of command and executes it, printing the results to the user. The program uses various data structures such as stacks, queues, hashmaps, and BPlus trees, and implements polymorphism through a Token class. Shunting Yard is used to handle conditions, and the state machine is represented by a 2D array.
</p>

[About](#about) •
[Technologies](#technologies) •
[Credits](#credits) •
[License](#license)

</div>

## About

<div align="center">

<img max-height=350 height=350 alt="Preview of DataQL Application" src="https://raw.githubusercontent.com/rparin/DataQL/main/preview/Preview.png">

</div>

### Features

- Allow user to save/read data from/to file
- Finite State Machine to parse user commands
- Create, load, insert, select, save, delete, and show tables

## Technologies

<div align="center">

[![SFML][sfml.io]][sfml-url]

</div>

## Credits

I made this application as a learning exercise.

## License

This project is licensed under the MIT License - see the [LICENSE][git-license-url] file for details.

<!-- MARKDOWN LINKS & IMAGES -->

[license.io]: https://img.shields.io/badge/license-MIT-blue.svg
[license-url]: https://opensource.org/licenses/MIT
[git-license-url]: https://github.com/rparin/DataQL/blob/main/LICENSE
[sfml.io]: https://img.shields.io/badge/sfml-f1f2ee?style=for-the-badge&logo=sfml
[sfml-url]: https://www.sfml-dev.org/
