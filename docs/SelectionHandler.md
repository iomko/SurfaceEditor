# SelectionHandler

### SelectionHandler<SelectCommandT>
Template class handling selection interactions for a given selection command type.

Inherits from InteractionHandler<SelectCommandT> and implements basic lifecycle methods for interaction handling.

###### Public Methods:
`SelectionHandler(SelectCommandT* command)`
Constructor that initializes the base InteractionHandler with the provided command pointer.

`void onBegin()`
Executes the associated command when the interaction begins.

`void onUpdate()`
Empty update handler (no operation during interaction update).

`void onEnd()`
Empty end handler (no operation when interaction ends).