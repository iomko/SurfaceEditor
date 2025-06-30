# CommandRegistry

The CommandRegistry class provides a centralized mechanism for registering and retrieving command instances at runtime.
It uses a string-based key system to manage command objects polymorphically via the CommandConcept base interface.

This class allows you to register any command type that provides a getCommandName() method, and later retrieve or clean up registered commands.

###### Public Methods
`template<typename CommandT, typename... Args> void registerCommand(Args&&... args)`
Registers a new command of type CommandT into the registry. The command must define a static method getCommandName() which returns a unique string identifier.
The provided arguments are perfectly forwarded to the constructor of the command.

`template<typename CommandT> CommandT* getCommand()`
Retrieves a pointer to a previously registered command of type CommandT using its getCommandName() as the lookup key.
Returns nullptr if the command is not found or not yet registered.

`void deleteRegistry()`
Deletes all registered command instances and clears the internal storage.
This should be called during shutdown to free memory and avoid leaks.
	