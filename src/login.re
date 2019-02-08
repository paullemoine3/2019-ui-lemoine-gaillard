let url_dev = "http://localhost:8080/";
type state = {
  email: string,
  password: string,
};

type action =
  | UpdateLogin(string)
  | UpdatePassword(string)
  | Login
  | Loading
  | LoadedUsers
  | LoadUsersFailed
  | Ok
  | Failure;

let login = state => {
  let stateUser = Js.Dict.empty();
  Js.Dict.set(stateUser, "email", Js.Json.string(state.email));
  Js.Dict.set(stateUser, "password", Js.Json.string(state.password));
  Js.Promise.(
    Fetch.fetchWithInit(
      url_dev ++ "api/v1/users/login",
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(
            Js.Json.stringify(Js.Json.object_(stateUser)),
          ),
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        (),
      ),
    )
    |> then_(Fetch.Response.json)
    |> then_(json =>
         Json.Decode.{email: field("email", string, json), password: ""}
         |> (user => Some(user) |> resolve)
       )
  );
};

let component = ReasonReact.reducerComponent("login");

let make = _children => {
  ...component,
  initialState: () => {email: "", password: ""},
  reducer: (action, state) =>
    switch (action) {
    | UpdateLogin(email) => ReasonReact.Update({...state, email})
    | UpdatePassword(password) => ReasonReact.Update({...state, password})
    | Login =>
      ReasonReact.UpdateWithSideEffects(
        state,
        self =>
          Js.Promise.(
            login(state)
            |> then_(result =>
                 switch (result) {
                 | Some(user) => resolve(self.send(LoadedUsers))
                 | None => resolve(self.send(LoadUsersFailed))
                 }
               )
            |> ignore
          ),
      )
    | _ => ReasonReact.Update(state)
    },
  render: _self =>
    <div>
      <h1> {ReasonReact.string("Page de login")} </h1>
      <form>
        <label>
          {ReasonReact.string("Loginsss")}
          <input
            type_="text"
            name="login"
            value={_self.state.email}
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
      <button onClick={_ => _self.send({Login})}>
        {ReasonReact.string("connect")}
      </button>
      <div> {ReasonReact.string(_self.state.email)} </div>
      <div> {ReasonReact.string(_self.state.password)} </div>
    </div>,
};
