try:
    import blackjack
except ImportError as e:
    print(f"Importing the shared library 'hangman' did not work.")
    print(f"Is (a link to) the shared library 'hangman.____.so' in the same directory as this python script?")
    print(f"The import caused the following exception '{e}'")
    print(f"Exiting")
    exit(1)

