type state = {
    grid: array(array(Unit.unitType))
};

let initialState = {
    grid: Array.make_matrix(4, 8, Unit.Road)
};

type coord = {
    rowId: int,
    colId: int
};

let gridReducer = (state, coord) => {
    state.grid[coord.rowId][coord.colId] = Unit.unitChange(state.grid[coord.rowId][coord.colId]);
    {grid: state.grid}
};


[@react.component]
let make = () => {
    let (state, dispatch) = React.useReducer(gridReducer, initialState);

    Js.log(state);
    state.grid
    |> Array.mapi((rowId, row) => 
        Array.mapi((colId, _) => 
            <Unit 
                key=("coord "++string_of_int(rowId)++" "++string_of_int(colId))
                id=("unit "++string_of_int(rowId)++" "++string_of_int(colId))
                toggleUnit=dispatch
                coord={rowId: rowId, colId: colId}
                unit=state.grid[rowId][colId]
                >
            </Unit>
        , row) 
        |> React.array)
    |> Array.mapi((rowId, row) =>
        <div 
            key=("row "++string_of_int(rowId))
            name=("row "++string_of_int(rowId))
            >
            {row}
        </div>
    )
    |> React.array;
};