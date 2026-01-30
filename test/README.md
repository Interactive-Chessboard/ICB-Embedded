## Tests on board
Connect the board to the computer running the tests

Run tests using the following command

```bash
pio test -e esp32s3-wroom-1-n16r8
```

Run a single folder at a time
```bash
pio test -e esp32s3-wroom-1-n16r8 -f folder_name
```

## Local tests
Run tests using the following command

```bash
pio test -e native
```

Run a single folder at a time
```bash
pio test -e native -f folder_name
```

## Coverage
See unit code coverage (main and hardware library are excluded since coverage is based on native tests)

Install Gcov Viewer extension

Run these following commands (ctrl + shift + p)
Gcov Viewer: Show                                                   - Show line coverage
Gcov Viewer: Hide                                                   - Hide line coverage
Gcov Viewer: Reset (Delete .gcda Files)                             - Resets the coverage
