type state = {
  errorTitle: string,
  error: string,
};

type action =
  | Error;

let component = ReasonReact.reducerComponent("ErrorPageConnection");

let make = _children => {
  ...component,
  initialState: () => {
    errorTitle: "Page d'erreur lors de la connection",
    error: "Une erreur est survenue : LOGIN :UNKOWN OR DELETED USER !",
  },
  reducer: (action, state) =>
    switch (action) {
    | Error => ReasonReact.NoUpdate
    },
  render: _self =>
    <div>
      <h1> {ReasonReact.string(_self.state.errorTitle)} </h1>
      <label> {ReasonReact.string(_self.state.error)} </label>
      <br />
    </div>,
};
