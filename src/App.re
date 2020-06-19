type state = {count: int};

type action =
  | Increment
  | Decrement;

let initialState = {count: 0};

let reducer = (state, action) =>
  switch (action) {
  | Increment => {count: state.count + 1}
  | Decrement => {count: state.count - 1}
  };

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);
  Random.init(int_of_float(Js.Date.now()));
  Js.log(Random.int(10));
  let a = [|[|1,2|],[|1,2|]|];
  Js.log(a);
  <main>
    {React.string("Oh hello")}
    <div>
      <button onClick={_ => dispatch(Decrement)}>
        {React.string("Decrement")}
      </button>
      <span className="counter">
        {state.count |> string_of_int |> React.string}
      </span>
      <button onClick={_ => dispatch(Increment)}>
        {React.string("Increment")}
      </button>
    </div>
    <div>
      <World>
      </World>
    </div>
  </main>
};
