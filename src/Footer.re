let component = ReasonReact.statelessComponent("Footer");

let make = (~todoCount, ~activeRoute, _children) => {
  ...component,
  render: _self =>
    <footer className="footer">
      <span className="todo-count">
        <strong> (ReasonReact.string(string_of_int(todoCount))) </strong>
        (
          switch (todoCount) {
          | 1 => ReasonReact.string(" item left")
          | _ => ReasonReact.string(" items left")
          }
        )
      </span>
      <ul className="filters">
        <li> <a className=(activeRoute == "/" ? "selected" : "") href="#/"> (ReasonReact.string("All")) </a> </li>
        <li>
          <a className=(activeRoute == "/active" ? "selected" : "") href="#/active">
            (ReasonReact.string("Active"))
          </a>
        </li>
        <li>
          <a className=(activeRoute == "/completed" ? "selected" : "") href="#/completed">
            (ReasonReact.string("Completed"))
          </a>
        </li>
      </ul>
    </footer>,
};
