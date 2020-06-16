[@react.component]
let make = (~label, ~onSubmit) => {
  let (editing, setEditing) = React.useState(() => false);
  let (value, onChange) = React.useState(() => label);
  let onCancel = _evt => setEditing(_ => false);
  let onFocus = event => ReactEvent.Focus.target(event)##select();
  
/*   React.useEffect0(
    () => {
      onChange(_ => label);
      Js.log("??");
      None
    }); */

  if (editing) {
    <form
      onSubmit={_ => {
        setEditing(_ => false);
        onSubmit(value);
      }}
      onBlur=onCancel>
      <input
        onBlur=onCancel
        onFocus
        onChange={
          event => {
            let value = ReactEvent.Form.target(event)##value;
            onChange(_ => value)
          }
        }
        value
      />
    </form>;
  } else {
    <span onDoubleClick={_evt => setEditing(_ => true)}>
      value->React.string
    </span>;
  };
};