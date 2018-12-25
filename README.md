Structure
---------

- Makefile       - Builds the project
- project/       - where the project code files are
- README.md      - The file you're reading
- tmp/           - Temporary files 

Build
------

`make` - builds the project
`make test` - builds the project with extra Flags (-Wall -Wextra -Werror)
`make clean` - deletes output files
`./sysprak_client -g <Game-ID> -p <Player-ID> [-c <Config File>]`

If you don't specify a config file the default (client.conf) will be used.
