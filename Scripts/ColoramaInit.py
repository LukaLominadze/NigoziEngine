try:
    from colorama import init, Fore, Back, Style
except ImportError as e:
    print(f'Error -> {e.msg}')
    print('Would you like to install colorama?')
    while True:
        answer = input('(y/n) -> ')
        if answer.lower() == 'n':
            sys.exit()
            break
        elif answer.lower() != 'y':
            print('Invalid input!')
            break
        else:
            out = subprocess.run(['py', '-m', 'pip', 'install', 'colorama'])
            print(out.stdout.decode())
            print("Restart the setup script to continue")
            input("Press any key to end setup...")
            sys.exit()
            break