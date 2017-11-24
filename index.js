const binding = require('bindings')('deasync');
const {
  getPromiseDetails,
  kPending,
  kFulfilled,
} = process.binding('util');

function wait(promise) {
  let state;
  let result;
  const fn = () => {
    [state, result] = getPromiseDetails(promise);
    return state === kPending;
  };
  while (fn()) {
    process._tickDomainCallback();
    binding.run();
  }
  if (state === kFulfilled)
    return result;
  else
    throw result;
}

module.exports = wait;
