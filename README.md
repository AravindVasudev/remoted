# Remoted

A simple daemon to perform system tasks such as shutdown, keypress, open links, etc via HTTP requests.

## Note

There are 2 versions of remoted:

    * Electron - works on all platform but heavy (about 110MB when packed)
    * C - very lite but platform dependant

## Building

    * C
        * Make the source

        ```
            $ make
        ```

        * Run remoted.out / remoted.exe

    * Electron
        * Install all dependencies 

        ```
            $ npm install
        ```

        * Package the application

        ```
            $ npm run package
        ```
        
        * Run the executable

    * The application will run in 0.0.0.0:8080
    * Set the executable as start-up to control it from start-up

## Routes

    * / - test route
    * /close - closes focused application (presses Alt + F4 or ⌘ + Q)
    * /shutdown - shutdowns the system
    * /mouse/circle - moves the mouse in circular path
    * /mouse/sine - moves the mouse in sine path (only in Electron)
    * /open?url=<url> - opens the URL in default browser


## License

Built under [MIT](./LICENSE) license.