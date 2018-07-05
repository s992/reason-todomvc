type todo = {
  id: int,
  title: string,
  complete: bool,
};

type state = {
  todos: list(todo),
  currentlyEditing: int,
};

type action =
  | Toggle(int)
  | ToggleAll(bool)
  | Edit(int)
  | Save(int, string)
  | Delete(int)
  | Create(string);

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {
    todos: [{id: 1, title: "Taste JavaScript", complete: true}, {id: 2, title: "Buy a unicorn", complete: false}],
    currentlyEditing: (-1),
  },
  reducer: (action, state: state) =>
    switch (action) {
    | Toggle(id) =>
      let updated = List.map(todo => todo.id == id ? {...todo, complete: ! todo.complete} : todo, state.todos);
      ReasonReact.Update({...state, todos: updated});
    | ToggleAll(toggled) =>
      let updated = List.map(todo => {...todo, complete: toggled}, state.todos);
      ReasonReact.Update({...state, todos: updated});
    | Edit(id) => ReasonReact.Update({...state, currentlyEditing: id})
    | Save(id, newValue) =>
      let updated = List.map(todo => todo.id == id ? {...todo, title: newValue} : todo, state.todos);
      ReasonReact.Update({currentlyEditing: (-1), todos: updated});
    | Delete(id) =>
      let updated = List.filter(todo => todo.id != id, state.todos);
      ReasonReact.Update({...state, todos: updated});
    | Create(title) =>
      let td = {id: List.length(state.todos) + 1, complete: false, title};
      ReasonReact.Update({...state, todos: List.append(state.todos, [td])});
    },
  render: self =>
    <div>
      <section className="todoapp">
        <Header onSave=(value => self.send(Create(value))) />
        <section className="main">
          <input
            id="toggle-all"
            className="toggle-all"
            type_="checkbox"
            onChange=(
              event => self.send(ToggleAll(ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##checked))
            )
          />
          <label htmlFor="toggle-all"> (ReasonReact.string("Mark all as complete")) </label>
          <ul className="todo-list">
            (
              self.state.todos
              |> List.map(todo =>
                   <TodoItem
                     key=(string_of_int(todo.id))
                     complete=todo.complete
                     title=todo.title
                     editing=(todo.id == self.state.currentlyEditing)
                     onToggle=(_event => self.send(Toggle(todo.id)))
                     onDestroy=(_event => self.send(Delete(todo.id)))
                     onEdit=(_event => self.send(Edit(todo.id)))
                     onSave=(newTitle => self.send(Save(todo.id, newTitle)))
                     onCancelEdit=(_event => self.send(Save(todo.id, todo.title)))
                   />
                 )
              |> Array.of_list
              |> ReasonReact.array
            )
          </ul>
        </section>
        <Footer todoCount=(self.state.todos |> List.filter(todo => ! todo.complete) |> List.length) />
      </section>
      <footer className="info">
        <p> (ReasonReact.string("Double-click to edit a todo")) </p>
        <p>
          (ReasonReact.string("Created by "))
          <a href="https://swalsh.org"> (ReasonReact.string("Sean Walsh")) </a>
        </p>
        <p> (ReasonReact.string("Part of ")) <a href="http://todomvc.com"> (ReasonReact.string("TodoMVC")) </a> </p>
      </footer>
    </div>,
};
