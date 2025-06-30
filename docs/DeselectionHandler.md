# DeselectionHandler
Handles user interaction for deselection-type operations.
Inherits from InteractionHandler and triggers deselection logic without requiring input parameters.

### DeselectionHandler<DeselectCommandT>

Templated on the type of deselection command it executes.

###### Public Methods:
`DeselectionHandler(DeselectCommandT* command)`
Constructor that initializes the handler with a pointer to a deselection command.

`void onBegin()`
Executes the associated deselection command when the interaction begins.

`void onUpdate()`
Empty override, no operation performed during update phase.

`void onEnd()`
Empty override, no operation performed at the end of the interaction.
