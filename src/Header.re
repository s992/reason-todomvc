type state = {value: string};

type action =
  | ValueChanged(string)
  | KeyPressed(int);

let component = ReasonReact.reducerComponent("Header");

let make = (~onSave, _children) => {
  ...component,
  initialState: () => {value: ""},
  reducer: (action, state: state) =>
    switch (action) {
    | ValueChanged(newValue) => ReasonReact.Update({value: newValue})
    | KeyPressed(13) =>
      onSave(state.value);
      ReasonReact.Update({value: ""});
    | KeyPressed(_) => ReasonReact.NoUpdate
    },
  render: self =>
    <header className="header">
      <h1> (ReasonReact.string("todos")) </h1>
      <input
        className="new-todo"
        placeholder="What needs to be done?"
        value=self.state.value
        onKeyDown=(event => self.send(KeyPressed(ReactEventRe.Keyboard.keyCode(event))))
        onChange=(
          event => self.send(ValueChanged(ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value))
        )
      />
    </header>,
};
