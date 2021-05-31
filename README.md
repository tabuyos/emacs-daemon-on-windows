# emacs-daemon-on-windows
Emacs daemon mode on Windows with Windows Service or Batch style.

# usage
## Windows Service Version
step 1: create windows service.
``` shell
sc create AutoEmacs binpath= create-service-binary-file-path obj= your-account-name password= your-account-password
```
step 2: allow your account to log in as a service in the local policy.
step 3: open your `emacsclientw` and test it!

## Batch Version
step 1: add [start-emacs.bat](./Batch/start-emacs.bat) to boot up.
step 2: open your `emacsclientw` and test it!

