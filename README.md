# Tree Editor

**Выполнил:** Темгаев Егор Витальевич

## Контакты

**Telegram:** @Giperbooola
**Телефон:** +7 910 892 5584
**Email:** [giperbola.cpp@gmail.com](mailto:giperbola.cpp@gmail.com)
**Резюме:** https://hh.ru/resume/9133f0d7ff0d6131490039ed1f65336a695331?hhtmFrom=applicant_profile

## Задание

Разработано приложение для работы с иерархическим деревом объектов.

### Реализовано

* хранение данных дерева в PostgreSQL;
* отображение дерева с помощью QWidget;
* фильтрация листьев по названию;
* фильтрация листьев по числовому значению;
* добавление листа;
* редактирование листа;
* удаление листа;
* удаление узла вместе с листьями;
* выгрузка дерева в файл;
* загрузка дерева из файла;
* отображение количества узлов и листьев;
* отображение минимального и максимального числового значения;
* unit-тесты.

## База данных

Для хранения данных используется PostgreSQL.

### Структура базы данных

```sql
CREATE TABLE nodes (
    id SERIAL PRIMARY KEY,
    name TEXT NOT NULL
);

CREATE TABLE leaves (
    id SERIAL PRIMARY KEY,
    node_id INTEGER NOT NULL REFERENCES nodes(id) ON DELETE CASCADE,
    name TEXT NOT NULL,
    value DOUBLE PRECISION NOT NULL
);
```

## Сборка

Для сборки используются:

* Qt 6.10.3 MinGW;
* MinGW 13.1.0;
* CMake.

Пути к Qt и MinGW указываются в `build.bat`:

```text
QT=C:\Qt\6.10.3\mingw_64
MINGW=C:\Qt\Tools\mingw1310_64
```

Для сборки необходимо запустить:

```bat
build.bat
```

Скрипт автоматически очищает директорию `build`, конфигурирует CMake и собирает проект.

После успешной сборки исполняемый файл находится в:

```text
build\TreeEditor.exe
```

### Подготовка Qt-зависимостей

Для запуска собранных `.exe` на другом компьютере можно использовать `windeployqt`:

```powershell
C:\Qt\6.10.3\mingw_64\bin\windeployqt.exe TreeEditor.exe
C:\Qt\6.10.3\mingw_64\bin\windeployqt.exe TreeManagerTest.exe
```

## Запуск

Для работы приложения должен быть установлен PostgreSQL.

Если приложение не может найти `libpq.dll`, необходимо добавить путь к папке `bin` PostgreSQL в `PATH`:

```powershell
$env:PATH += ";C:\Program Files\PostgreSQL\18\bin"
.\TreeEditor.exe
```

Путь может отличаться в зависимости от версии и расположения PostgreSQL.

## Запуск тестов

Для тестов используется база данных `treeeditor_test`.

```powershell
$env:TEST_DB_HOST = "localhost"
$env:TEST_DB_PORT = "5432"
$env:TEST_DB_NAME = "treeeditor_test"
$env:TEST_DB_USER = "postgres"
$env:TEST_DB_PASSWORD = "1234"

.\TreeManagerTest.exe
```

## Готовая сборка

В архиве `build.zip` находится уже собранная версия программы и unit-тестов для Windows.
