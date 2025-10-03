# InteractionHandler

### InteractionHandler<CommandT, IParams = OpParams> class
Template class providing a base implementation of InteractionHandlerConcept for commands of type CommandT and input parameters of type IParams.  

######  Possible Methods:
`virtual void onBegin()` - Pure virtual method called at the beginning of an interaction without parameters.  
`virtual void onUpdate()` - Pure virtual method called during interaction updates without parameters.  
`virtual void onEnd()` - Pure virtual method called at the end of an interaction without parameters.  

`virtual void onBegin(const OpParams& iParams)` - Pure virtual method called at the beginning of an interaction with parameters.  
`virtual void onUpdate(const OpParams& iParams)` - Pure virtual method called during interaction updates with parameters.  
`virtual void onEnd(const OpParams& iParams)` - Pure virtual method called at the end of an interaction with parameters.  

`virtual CommandConcept* getCommand() const` - Returns a pointer to the associated command.  

#### All the InteractionHandler class instances used in the project:

`DeselectionHandler` – Interaction handler for handling deselections in the scene.    
`SelectionHandler` – Interaction handler for handling selections in the scene.  
`BrushInteractionHandler` - Interaction handler for handling surface deformations.  
