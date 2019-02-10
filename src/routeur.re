type page =
  | Login
  | Register
  | Score
  | ErrorCo
  | ErrorRegister;

module Config = {
  type route = page;
  let toRoute = (url: ReasonReact.Router.url) =>
    switch (url.path) {
    | ["login"] => Login
    | ["register"] => Register
    | ["score"] => Score
    | ["errorCo"] => ErrorCo
    | ["errorRegister"] => ErrorRegister
    | _ => Login
    };
  let toUrl = route =>
    switch (route) {
    | Login => "/"
    | Register => "/register"
    | Score => "/score"
    | ErrorCo => "/errorCo"
    | ErrorRegister => "/errorRegister"
    };
};

let str = ReasonReact.string;

type state = {route: page};
type action =
  | ChangePage(page);

let component = ReasonReact.reducerComponent("Routeur");
let make = _children => {
  ...component,
  initialState: () => {
    route: ReasonReact.Router.dangerouslyGetInitialUrl() |> Config.toRoute,
  },
  reducer: (action, _state) =>
    switch (action) {
    | ChangePage(route) => ReasonReact.Update({route: route})
    },
  didMount: self => {
    let watcherId =
      ReasonReact.Router.watchUrl(url =>
        self.send(ChangePage(Config.toRoute(url)))
      );
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherId));
  },
  render: self =>
    <div>
      {switch (self.state.route) {
       | Login => <Login />
       | Register => <Register />
       | Score => <Score />
       | ErrorCo => <ErrorCo />
       | ErrorRegister => <ErrorRegister />
       }}
      <a href="login"> {str("Login")} </a>
      <a href="register"> {str("register")} </a>
    </div>,
};
