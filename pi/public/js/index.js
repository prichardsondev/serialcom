let socket = io();

let soundCount = 0;
let maxSoundCount = 1000;

socket.on ('updateClient', data => console.log(data));

socket.on('statedata', data => {
    console.log(data);
    schoolA.push(data);
    gridOptions.api.setRowData(schoolA);
});

const columnDefs = [{field: 'school'}, { field: 'light' }, { field: 'temp' }, { field: 'timestamp' }];

// specify the data
const schoolA = [

];

// let the grid know which columns and what data to use
const gridOptions = {
  defaultColDef: {
    sortable: true,
    
    filter: 'agTextColumnFilter',
    resizable: true
  },  
  pagination: true,
  columnDefs: columnDefs,
  rowData: schoolA,
  rowSelection: 'single',
  animateRows: true,
};


// setup the grid after the page has finished loading
document.addEventListener('DOMContentLoaded', function () {
  var gridDiv = document.querySelector('#myGrid');
  new agGrid.Grid(gridDiv, gridOptions);
});

let cTime = () => {
  let currentDate = new Date();
  return `${currentDate.getHours()}:${currentDate.getMinutes()}:${currentDate.getSeconds()}`;
}