open Decoder;

let url_dev: string = "http://localhost:8080";
type state = {
  email: string,
  password: string,
};

type action =
  | UpdateLogin(string)
  | UpdatePassword(string)
  | Register
  | RegisterUsers
  | RegisterUsersFailed;

let component = ReasonReact.reducerComponent("Register");

let register = state => {
  let stateUser = Js.Dict.empty();
  Js.Dict.set(stateUser, "email", Js.Json.string(state.email));
  Js.Dict.set(stateUser, "password", Js.Json.string(state.password));
  Js.Promise.(
    Fetch.fetchWithInit(
      url_dev ++ "api/v1/users/",
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
    |> then_(json => {
         Js.log(json);
         json |> Decoder.decodeResponse |> (user => Some(user) |> resolve);
       })
  );
};

let make = _children => {
  ...component,
  initialState: () => {email: "", password: ""},
  reducer: (action, state) =>
    switch (action) {
    | UpdateLogin(email) => ReasonReact.Update({...state, email})
    | UpdatePassword(password) => ReasonReact.Update({...state, password})
    | Register =>
      ReasonReact.UpdateWithSideEffects(
        state,
        self =>
          Js.Promise.(
            register(state)
            |> then_(result =>
                 switch (result) {
                 | Some(user) => resolve(self.send(RegisterUsers))
                 }
               )
            |> catch(_err => {
                 Js.log(_err);
                 Js.Promise.resolve(self.send(RegisterUsersFailed));
               })
            |> ignore
          ),
      )
    | RegisterUsers =>
      ReasonReact.SideEffects(_ => ReasonReact.Router.push("score"))
    | RegisterUsersFailed =>
      ReasonReact.SideEffects(_ => ReasonReact.Router.push("errorRegister"))
    | _ => ReasonReact.Update(state)
    },
  render: _self =>
    <div>
      <h1> {ReasonReact.string("Page d'enregistrement")} </h1>
      <form>
        <label>
          {ReasonReact.string("Login")}
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
      <button onClick={_ => _self.send({Register})}>
        {ReasonReact.string("register")}
      </button>
    </div>,
};