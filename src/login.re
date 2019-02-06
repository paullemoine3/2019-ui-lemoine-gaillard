type state = {
  login: string,
  password: string,
};

type action =
  | UpdateLogin(string)
  | UpdatePassword(string);

let component = ReasonReact.reducerComponent("Login");

ReasonReact.Router.push("login");

let make = _children => {
  ...component,
  initialState: () => {login: "", password: ""},
  reducer: (action, state) =>
    switch (action) {
    | UpdateLogin(login) => ReasonReact.Update({...state, login})
    | UpdatePassword(password) => ReasonReact.Update({...state, password})
    },
  render: _self =>
    <div>
      <h1> {ReasonReact.string("Page de login")} </h1>
      <form>
        <label>
          {ReasonReact.string("Login")}
          <input
            type_="text"
            name="login"
            value={_self.state.login}
            onChange={event =>
              _self.send(UpdateLogin(ReactEvent.Form.target(event)##value))
            }
          />
        </label>
        <br />
        <label>
          {ReasonReact.string("mdp")}
          <input
            type_="text"
            name="mdp"
            value={_self.state.password}
            onChange={event =>
              _self.send(
                UpdatePassword(ReactEvent.Form.target(event)##value),
              )
            }
          />
        </label>
        <br />
      </form>
      <button onClick={_ => _self.send(Register)}>
        {ReasonReact.string(" register ")}
      </button>
      <div> {ReasonReact.string(_self.state.login)} </div>
      <div> {ReasonReact.string(_self.state.password)} </div>
    </div>,
};