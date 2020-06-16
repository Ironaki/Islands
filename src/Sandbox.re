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
  /* unused variables are prefixed with an underscore */
  let onSubmit = _event => {
    Js.log("Hello this is a log!")
  };
   

  /* onSubmit=onSubmit turns to just onSubmit */
  <form onSubmit>
      <input
        /* class names work the same way */
        className="w-full"
        /* type_ is underscored b/c its a reserved word in Reason */
        type_="text"
        /* No brackets needed! */
        autoFocus=true
        placeholder="Game Code"
      />
      <button type_="submit"> {React.string("Button label")} </button>
    </form>;
};