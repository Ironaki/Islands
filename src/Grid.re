open SharedType;
open Unit;

type state = {
  startCoord: option(coord),
  endCoord: option(coord),
  grid: array(array(unitType)),
  path: option(list(coord)),
};

// Initial State is Tokyo Bay
let initialState = {
  startCoord: Some(TokyoBay.tokyoBayStart),
  endCoord: Some(TokyoBay.tokyoBayEnd),
  grid: TokyoBay.tokyoBayGrid(),
  path: None,
};

let blankState = (rowSize, colSize) => {
  let base = Array.make_matrix(rowSize, colSize, Land(Road, Ordinary, NotPath));
  base[0][0] = Land(Road, Start, NotPath);
  base[rowSize - 1][colSize - 1] = Land(Road, End, NotPath);
  {
    startCoord: Some({row: 0, col: 0}),
    endCoord: Some({row: rowSize - 1, col: colSize - 1}),
    grid: base,
    path: None,
  };
};

let randomState = (rowSize, colSize) => {
  let base = Array.make_matrix(rowSize, colSize, Land(Road, Ordinary, NotPath));
  let _ = Array.mapi((rowId, row) => Array.mapi((colId, _) => base[rowId][colId] = randomUnitInit(), row), base);
  base[0][0] = randomStart();
  base[rowSize - 1][colSize - 1] = randomEnd();
  {
    startCoord: Some({row: 0, col: 0}),
    endCoord: Some({row: rowSize - 1, col: colSize - 1}),
    grid: base,
    path: None,
  };
};

type action =
  | Toggle(coord)
  | TogglePath
  | EnableSetStart // When click on "set start" button
  | EnableSetEnd //When click on "set end" button
  | SetStart(coord) // When click on a unit to change it to start
  | SetEnd(coord) // When click on a unit to change it to end
  | Reconstruct(initType, int, int);

let exist = (op: option('a)) => {
  switch (op) {
  | None => false
  | Some(_) => true
  };
};

// USE WITH CAUTION
// To avoid compiler warning of not handling None case for option
// This function will log Error when None branch is reached. Please debug then.
let getCoordOption = (op: option(coord)) => {
  switch (op) {
  | None =>
    Js.log("Error: getCoordOption should be only applied to option(coord) that could not be None");
    {row: (-1), col: (-1)};
  | Some(coord) => coord
  };
};

let gridReducer = (state, action) => {
  switch (action) {
  | Toggle(coord) =>
    state.grid[coord.row][coord.col] = unitToggle(state.grid[coord.row][coord.col]);
    {...state, grid: state.grid};
  | TogglePath =>
    let (resPath, nextPath) =
      switch (state.path) {
      | None =>
        let p = AStar.aStar(state.grid, getCoordOption(state.startCoord), getCoordOption(state.endCoord));
        (p, Some(p));
      | Some(p) => (p, None)
      };
    let _ =
      resPath |> List.map(coord => state.grid[coord.row][coord.col] = togglePath(state.grid[coord.row][coord.col]));
    {...state, grid: state.grid, path: nextPath};
  | EnableSetStart =>
    let start_ = getCoordOption(state.startCoord);
    state.grid[start_.row][start_.col] = setOrdinary(state.grid[start_.row][start_.col]);
    {...state, startCoord: None, grid: state.grid};
  | EnableSetEnd =>
    let end_ = getCoordOption(state.endCoord);
    state.grid[end_.row][end_.col] = setOrdinary(state.grid[end_.row][end_.col]);
    {...state, endCoord: None, grid: state.grid};
  | SetStart(coord) =>
    state.grid[coord.row][coord.col] = setStart(state.grid[coord.row][coord.col]);
    {...state, startCoord: Some(coord), grid: state.grid};
  | SetEnd(coord) =>
    state.grid[coord.row][coord.col] = setEnd(state.grid[coord.row][coord.col]);
    {...state, endCoord: Some(coord), grid: state.grid};
  | Reconstruct(init, rowSize, colSize) =>
    switch (init) {
    | Blank => blankState(rowSize, colSize)
    | Random => randomState(rowSize, colSize)
    | TokyoBay => {
        startCoord: Some(TokyoBay.tokyoBayStart),
        endCoord: Some(TokyoBay.tokyoBayEnd),
        grid: TokyoBay.tokyoBayGrid(),
        path: None,
      }
    }
  };
};

[@react.component]
let make = (~rowSize, ~colSize, ~init, ~reconstructable, ~inputValid) => {
  let (state, dispatch) = React.useReducer(gridReducer, initialState);
  <div className="grid column">
    <div className="searchButton">
      <button
        onClick={_ => dispatch(EnableSetStart)}
        disabled={exist(state.path) || !exist(state.startCoord) || !exist(state.endCoord)}>
        {React.string("Set Start")}
      </button>
      <button onClick={_ => dispatch(TogglePath)} disabled={!exist(state.startCoord) || !exist(state.endCoord)}>
        {exist(state.path) ? React.string("Clear Path") : React.string("Find Path")}
      </button>
      <button
        onClick={_ => dispatch(EnableSetEnd)}
        disabled={exist(state.path) || !exist(state.startCoord) || !exist(state.endCoord)}>
        {React.string("Set End")}
      </button>
    </div>
    {switch (state.path, exist(state.startCoord), exist(state.endCoord)) {
     | (None, true, true) =>
       <h4> {React.string("Click Buttons Above for Different Functions or On the Map to Toggle the Terrain")} </h4>
     | (None, false, _) => <h4> {React.string("Set Start by Clicking on the Map (Cannot set start on Water)")} </h4>
     | (None, _, _) => <h4> {React.string("Set End by Clicking on the Map (Cannot set start on Water)")} </h4>
     | (Some(p), _, _) =>
       switch (p) {
       | [] => <h4> {React.string("The goal is not reachable :(")} </h4>
       | _ => <h4> {React.string("Found the least-cost path for you :)")} </h4>
       }
     }}
    {state.grid
     |> Array.mapi((rowId, row) =>
          row
          |> Array.mapi((colId, _) =>
               <Unit
                 key={"coord " ++ coordStr({row: rowId, col: colId})}
                 id={"unit " ++ coordStr({row: rowId, col: colId})}
                 action={
                   switch (!exist(state.startCoord), !exist(state.endCoord)) {
                   | (true, _) => SetStart({row: rowId, col: colId})
                   | (_, true) => SetEnd({row: rowId, col: colId})
                   | _ => Toggle({row: rowId, col: colId})
                   }
                 }
                 dispatch
                 unit={Array.get(state.grid[rowId], colId)}
                 foundPath={exist(state.path)}
                 startEndNotSet={!exist(state.startCoord) || !exist(state.endCoord)}
               />
             )
          |> React.array
        )
     |> Array.mapi((rowId, row) =>
          <div className="grid row" key={"row " ++ string_of_int(rowId)} name={"row " ++ string_of_int(rowId)}>
            row
          </div>
        )
     |> React.array}
    {switch (reconstructable, inputValid) {
     | (false, _) =>
       <div className="reconstruct-world"> <h5> {React.string("Change World with the Button Below")} </h5> </div>
     | (true, false) =>
       <div className="reconstruct-world">
         <h5> {React.string("Please Enter Width (3 ~ 96), Height (3 ~ 54)")} </h5>
       </div>
     | (true, true) =>
       <div className="reconstruct-world">
         <button onClick={_ => dispatch(Reconstruct(init, rowSize, colSize))}>
           {React.string("Build New World")}
         </button>
       </div>
     }}
  </div>;
};
