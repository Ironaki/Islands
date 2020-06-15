/* we create a type for the action */
type action =
  | Tick;

/* and do the same for state */
type state = {count: int};

let initialState = {count: 0};

let reducer =  (state, action) => {
    switch (action) {
        | Tick => {count: state.count + 1}
    }
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  /* useEffect hook takes 0 arguments hence, useEffect0 */
  React.useEffect0(() => {
    let timerId = Js.Global.setInterval(() => dispatch(Tick), 1000);
    Some(() => Js.Global.clearInterval(timerId));
  });

  /* ints need to be converted to strings, that are then consumed by React.string */
  <div> {React.string(string_of_int(state.count))} </div>;
};