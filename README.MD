# Курсовой проект «Парсер INI-файлов»
[Условие задачи](https://github.com/netology-code/cppl-diplom)

Проект состоит из компонентов:
 - IniFileParser - динамическая библиотека, которая содержит класс IniParser
     - IniFileParser cmake target - сама библиотека
     - IniFileParserTests cmake target - тесты на Catch2
 - IniFileParserApp - демонстрационное приложение, которое вызывает IniParser
    - IniFileParserApp cmake target