function on_dropdown_toggle(element) {
  console.log("enter: on_dropdown_toggle");

  /* Get all "nav-menu-level-2" items. */
  var elements = document.getElementsByClassName("nav-menu-level-2");
  for (let i = 0;i < elements.length; ++i) {
    let element = elements[i];
    element.classList.remove("opened");
    element.classList.add("closed");
  }
  
  /*Get the adjacent "ul" element of the "a" element and toggle its visibility.*/
  sibling = element.nextSibling;
  sibling.classList.toggle("closed");
  sibling.classList.toggle("opened");
  
  console.log("leave: on_dropdown_toggle");
}
