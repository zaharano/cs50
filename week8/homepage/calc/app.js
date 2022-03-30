const calc = {
  currVal: '',
  opVal: '',
  opSign: '',
  chainFlag: false,

  plus() {
    return String(Number(this.opVal) + Number(this.currVal));
  },
  minus() {
    return String(Number(this.opVal) - Number(this.currVal));
  },
  times() {
    return String(Number(this.opVal) * Number(this.currVal));
  },
  divide() {
    if (Number(this.currVal) === 0) {
      return 'bananas';
    } else return String(Number(this.opVal) / Number(this.currVal));
  },
  equals() {
    const { opSign } = this;
    if (opSign) {
      this.currVal = this[opSign]();
      this.endOp();
      this.chainFlag = true;
      // error reset
      if (this.currVal === 'bananas') {
        this.currVal = '';
      }
    }
  },
  endOp() {
    this.opVal = '';
    this.opSign = '';
    display.update();
    display.opClear();
  },
  clear() {
    this.currVal = '';
    this.endOp();
    this.chainFlag = false;
  },
  addDigit(newDigit) {
    // stops leading zeroes
    if (newDigit === '0' && this.currVal === '') {
      return;
    }
    // clears currVal if it exists as a result
    if (this.chainFlag) {
      this.currVal = '';
      this.chainFlag = false;
    }
    this.currVal = this.currVal + newDigit;
    display.update();
  },
  fireOp(newOp) {
    const { opVal, currVal } = this;

    // if neither exists, quit
    if (!opVal && !currVal) {
      return;
      // if only opVal exists, reset op and quit
    } else if (opVal && !currVal) {
      this.setOp(newOp);
      return;
      // if both values already exist, equals then...
    } else if (opVal && currVal) {
      this.equals();
    } // if only currVal exists, then...
    // advance to op stage
    this.setOp(newOp);
    this.opVal = this.currVal;
    this.currVal = '';
    display.update();
  },
  setOp(newOp) {
    this.opSign = newOp;
    display.op();
  },
  point() {
    const { currVal } = this;
    // if no current value exists
    // OR current value exists as a result
    // start with '0.'
    if (!currVal || this.chainFlag) {
      this.chainFlag = false;
      this.currVal = '0.';
    }
    // disallow second .
    else if (currVal.includes('.')) {
      return;
    }
    // base case of adding a . to value string
    else {
      this.currVal = currVal + '.';
    }
    display.update();
  },
  neg() {
    const { currVal } = this;
    // avoids weird crap, may re-add ability to neg these
    if (!currVal || this.chainFlag) {
      return;
    }
    this.currVal = String(currVal * -1);
    display.update();
  },
  delete() {
    const { currVal } = this;
    if (!currVal || this.chainFlag) {
      return;
    }
    this.currVal = currVal.slice(0, currVal.length - 1);
    display.update();
  },
};

const display = {
  // limit on value display
  // calc handles and maintains values offscreen outside this limits
  MAX_DIGITS: 11,

  update() {
    let mainVal = this.digitFix(calc.currVal);
    let subVal = this.digitFix(calc.opVal);
    // display 0 for #main if value is empty
    document.querySelector('#main').textContent = mainVal ? mainVal : '0';
    document.querySelector('#sub').textContent = subVal ? subVal : '';
  },
  // rounds values with large numbers of digits for display
  // does not actually round the values in calc!
  digitFix(numString) {
    // if num is not over max, return it
    if (numString.length < this.MAX_DIGITS) {
      return numString;
    }
    // removes precision in display to accomodate extra chars
    // like from -, ., or exponentiation
    let realDigits = this.MAX_DIGITS;
    while (
      String(Number(numString).toPrecision(realDigits)).length > this.MAX_DIGITS
    ) {
      realDigits--;
    }
    return String(Number(numString).toPrecision(realDigits));
  },
  op() {
    display.opClear();
    let active = document.querySelector(`li .fa-${calc.opSign}`);
    if (active) {
      active.classList.add('active');
    }
  },
  opClear() {
    let deactivate = document.querySelector(`.active`);
    if (deactivate) {
      deactivate.classList.remove('active');
    }
  },
};

const controls = {
  keymap: {
    '+': 'plus',
    '-': 'minus',
    '*': 'times',
    '/': 'divide',
    '.': 'point',
    Backspace: 'delete',
    Enter: 'equals',
    n: 'neg',
    N: 'neg',
    c: 'clear',
    C: 'clear',
    0: '0',
    1: '1',
    2: '2',
    3: '3',
    4: '4',
    5: '5',
    6: '6',
    7: '7',
    8: '8',
    9: '9',
  },
  init() {
    let numButts = document.querySelectorAll('.num');
    for (let button of numButts) {
      button.addEventListener('click', function () {
        calc.addDigit(this.id);
      });
    }
    let opButts = document.querySelectorAll('.op');
    for (let button of opButts) {
      button.addEventListener('click', function () {
        calc.fireOp(this.id);
      });
    }
    let otherButts = document.querySelectorAll('.other');
    for (let button of otherButts) {
      button.addEventListener('click', function () {
        calc[this.id]();
      });
    }
    window.addEventListener('keypress', (e) => {
      let sel = document.getElementById(controls.keymap[e.key]);
      if (sel) sel.click();
    });
  },
};

display.update();
controls.init();
