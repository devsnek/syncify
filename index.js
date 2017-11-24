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
    const details = getPromiseDetails(promise);
    if (details === undefined) {
      result = promise;
      return false;
    }
    [state, result] = details;
    return state === kPending;
  };
  while (fn()) {
    process._tickDomainCallback();
    binding.run();
  }
  if (state === undefined || state === kFulfilled)
    return result;
  else
    throw result;
}

module.exports = wait;
