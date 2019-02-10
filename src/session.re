let saveTokenToStorage = value => {
  Dom.Storage.(localStorage |> setItem("Authorization", value));
};

let getTokenFromStorage = () => {
  Dom.Storage.(localStorage |> getItem("Authorization"));
};
