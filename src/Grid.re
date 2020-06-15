let grid = Array.make_matrix(3, 5, Unit.Road);




[@react.component]
let make = () => {
    Js.log(grid);
    grid
    |> Array.mapi((rowId, row) => 
        Array.mapi((colId, _) => 
            <Unit 
                key=("coord "++string_of_int(rowId)++" "++string_of_int(colId))
                id=("unit "++string_of_int(rowId)++" "++string_of_int(colId))
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