Current version uses OS-specific operations, and perhaps it's extra/unneeded. Though it is independent
    If `system()` were to be introduced, the program assumes a shell exists and it can detect it. Makes program unusable if the user is using an unknown shell.
