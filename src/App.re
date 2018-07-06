type todo = {
  id: int,
  title: string,
  complete: bool,
};

type visibility =
  | All
  | Active
  | Complete;

type state = {
  todos: list(todo),
  currentlyEditing: int,
  visible: visibility,
};

type action =
  | Toggle(int)
  | ToggleAll(bool)
  | Edit(int)
  | Save(int, string)
  | Delete(int)
  | Create(string)
  | ToggleVisibility(visibility);

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {
    todos: [{id: 1, title: "Taste JavaScript", complete: true}, {id: 2, title: "Buy a unicorn", complete: false}],
    currentlyEditing: (-1),
    visible: All,
  },
  reducer: (action, state: state) =>
    switch (action) {
    | Toggle(id) =>
      ReasonReact.Update({
        ...state,
        todos: List.map(todo => todo.id == id ? {...todo, complete: ! todo.complete} : todo, state.todos),
      })
    | ToggleAll(toggled) =>
      ReasonReact.Update({...state, todos: List.map(todo => {...todo, complete: toggled}, state.todos)})
    | Edit(id) => ReasonReact.Update({...state, currentlyEditing: id})
    | Save(id, newValue) =>
      ReasonReact.Update({
        ...state,
        currentlyEditing: (-1),
        todos: List.map(todo => todo.id == id ? {...todo, title: newValue} : todo, state.todos),
      })
    | Delete(id) => ReasonReact.Update({...state, todos: List.filter(todo => todo.id != id, state.todos)})
    | Create(title) =>
      let newTodo = {id: List.length(state.todos) + 1, complete: false, title};
      ReasonReact.Update({...state, todos: List.append(state.todos, [newTodo])});
    | ToggleVisibility(vis) => ReasonReact.Update({...state, visible: vis})
    },
  didMount: self => {
    let watcherId =
      ReasonReact.Router.watchUrl(url =>
        switch (url.hash) {
        | "/active" => self.send(ToggleVisibility(Active))
        | "/completed" => self.send(ToggleVisibility(Complete))
        | _ => self.send(ToggleVisibility(All))
        }
      );

    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherId));
  },
  render: self => {
    let todos =
      List.filter(
        todo =>
          switch (self.state.visible) {
          | All => true
          | Active => ! todo.complete
          | Complete => todo.complete
          },
        self.state.todos,
      );

    <div>
      <section className="todoapp">
        <Header onSave=(value => self.send(Create(value))) />
        (
          List.length(todos) <= 0 ?
            ReasonReact.null :
            <section className="main">
              <input
                id="toggle-all"
                className="toggle-all"
                type_="checkbox"
                onChange=(
                  event =>
                    self.send(ToggleAll(ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##checked))
                )
              />
              <label htmlFor="toggle-all"> (ReasonReact.string("Mark all as complete")) </label>
              <ul className="todo-list">
                (
                  todos
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
        )
        (
          List.length(todos) > 0 ?
            <Footer
              todoCount=(todos |> List.filter(todo => ! todo.complete) |> List.length)
              activeRoute=(
                switch (self.state.visible) {
                | All => "/"
                | Active => "/active"
                | Complete => "/completed"
                }
              )
            /> :
            ReasonReact.null
        )
      </section>
      <footer className="info">
        <p> (ReasonReact.string("Double-click to edit a todo")) </p>
        <p>
          (ReasonReact.string("Created by "))
          <a href="https://swalsh.org"> (ReasonReact.string("Sean Walsh")) </a>
        </p>
        <p> (ReasonReact.string("Part of ")) <a href="http://todomvc.com"> (ReasonReact.string("TodoMVC")) </a> </p>
      </footer>
    </div>;
  },
};
