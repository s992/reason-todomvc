type state = {
  value: string,
  inputRef: ref(option(Dom.element)),
};
type retainedProps = {editing: bool};

type action =
  | ValueChanged(string)
  | KeyPressed(int);

let setInputRef = (ref, {ReasonReact.state}) => state.inputRef := Js.Nullable.toOption(ref);

let component = ReasonReact.reducerComponentWithRetainedProps("TodoItem");

let make = (~complete, ~title, ~editing, ~onToggle, ~onDestroy, ~onEdit, ~onSave, ~onCancelEdit, _children) => {
  ...component,
  initialState: () => {value: "", inputRef: ref(None)},
  reducer: (action, state: state) =>
    switch (action) {
    | ValueChanged(newValue) => ReasonReact.Update({...state, value: newValue})
    | KeyPressed(13) =>
      onSave(state.value);
      ReasonReact.Update({...state, value: ""});
    | KeyPressed(27) =>
      onCancelEdit();
      ReasonReact.Update({...state, value: ""});
    | KeyPressed(_) => ReasonReact.NoUpdate
    },
  retainedProps: {
    editing: editing,
  },
  didUpdate: ({oldSelf, newSelf}) =>
    if (! oldSelf.retainedProps.editing && newSelf.retainedProps.editing) {
      switch (newSelf.state.inputRef^) {
      | None => ()
      | Some(r) =>
        let node = ReactDOMRe.domElementToObj(r);
        ignore(node##focus());
      };
    },
  render: self =>
    <li className=([complete ? "completed" : "", editing ? "editing" : ""] |> String.concat(" "))>
      <div className="view">
        <input className="toggle" type_="checkbox" checked=complete onChange=onToggle />
        <label onDoubleClick=onEdit> (ReasonReact.string(title)) </label>
        <button className="destroy" onClick=onDestroy />
      </div>
      (
        editing ?
          <input
            className="edit"
            ref=(self.handle(setInputRef))
            value=self.state.value
            onKeyDown=(event => self.send(KeyPressed(ReactEventRe.Keyboard.keyCode(event))))
            onBlur=(_event => onSave(self.state.value))
            onChange=(
              event => self.send(ValueChanged(ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value))
            )
          /> :
          ReasonReact.null
      )
    </li>,
};
