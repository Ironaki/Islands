open SharedType;

type state = {
    grid: array(array(Unit.unitType))
};

let initialState = {
    grid: Array.make_matrix(4, 8, Unit.Road)
};

type action = 
    | Toggle(coord)
    | FindPath;

let gridReducer = (state, action) => {
    switch (action) {
        | Toggle(coord) => {
            state.grid[coord.row][coord.col] = Unit.unitChange(state.grid[coord.row][coord.col]);
            {grid: state.grid}
        }
        | FindPath => {
            Js.log(AStar.aStar(state.grid));
            state
        }
    }
};


[@react.component]
let make = () => {
    let (state, dispatch) = React.useReducer(gridReducer, initialState);

    Js.log(state);

    <div className="grid">
        <button onClick=(_ => dispatch(FindPath))> 
            {React.string("Find path")}
        </button> 
        {state.grid
        |> Array.mapi((rowId, row) => 
            Array.mapi((colId, _) => 
                <Unit 
                    key=("coord "++string_of_int(rowId)++" "++string_of_int(colId))
                    id=("unit "++string_of_int(rowId)++" "++string_of_int(colId))
                    action=Toggle({row: rowId, col: colId})
                    toggleUnit=dispatch
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
        |> React.array;}
    </div>
};