open SharedType;

type state = {
  row: int,
  col: int,
  showEditInfo: bool,
  init: initType,
};

type action =
  | ToggleEditInfo
  | SetCol(int)
  | SetRow(int)
  | ToggleInit(string);

let worldReducer = (state, action) => {
  switch (action) {
  | ToggleEditInfo => {row: state.row, col: state.col, showEditInfo: !state.showEditInfo, init: state.init}
  | SetCol(value) => {row: state.row, col: value, showEditInfo: state.showEditInfo, init: state.init}
  | SetRow(value) => {row: value, col: state.col, showEditInfo: state.showEditInfo, init: state.init}
  | ToggleInit(init_) => {
      row: state.row,
      col: state.col,
      showEditInfo: state.showEditInfo,
      init:
        switch (init_) {
        | "Blank" => Blank
        | "Random" => Random
        | "TokyoBay" => TokyoBay
        | _ => TokyoBay
        },
    }
  };
};

let initialSate = {row: 54, col: 96, showEditInfo: false, init: Blank};

let inputValid = (row, col) =>
  if (row >= 3 && row <= 54 && col >= 3 && col <= 96) {
    true;
  } else {
    false;
  };

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(worldReducer, initialSate);
  /*  Js.log(state); */
  <div className="world">
    <Grid
      rowSize={state.row}
      colSize={state.col}
      init={state.init}
      reconstructable={state.showEditInfo && inputValid(state.row, state.col)}
    />
    <div className="settingWorld">
      <div>
        <button onClick={_ => dispatch(ToggleEditInfo)}>
          {switch (state.showEditInfo) {
           | false => React.string("Set World")
           | true => React.string("Cancel Set World")
           }}
        </button>
      </div>
      {if (state.showEditInfo) {
         <div>
           <form className="set-world-form">
             <label>
               {React.string("Enter Width: ")}
               <input
                 type_="number"
                 value={string_of_int(state.col)}
                 onChange={event => {
                   let value = ReactEvent.Form.target(event)##value;
                   switch (value) {
                   | "" => dispatch(SetCol(0))
                   | _ => dispatch(SetCol(int_of_string(value)))
                   };
                 }}
               />
             </label>
             <label>
               {React.string("Enter Height: ")}
               <input
                 type_="number"
                 value={string_of_int(state.row)}
                 onChange={event => {
                   let value = ReactEvent.Form.target(event)##value;
                   switch (value) {
                   | "" => dispatch(SetRow(0))
                   | _ => dispatch(SetRow(int_of_string(value)))
                   };
                 }}
               />
             </label>
             <select onChange={event => dispatch(ToggleInit(ReactEvent.Form.target(event)##value))}>
               <option value="Blank"> {React.string("Blank")} </option>
               <option value="Random"> {React.string("Random")} </option>
               <option value="TokyoBay"> {React.string("Back to Tokyo Bay")} </option>
             </select>
             {if (inputValid(state.row, state.col)) {
                <div />;
              } else {
                <div> {React.string("Valid Width: 3-96 Valid Height 3-54")} </div>;
              }}
           </form>
         </div>;
       } else {
         <div />;
       }}
    </div>
  </div>;
};
