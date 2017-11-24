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
      state = kFulfilled;
      result = promise;
      return false;
    }
    state = details[0];
    result = details[1];
    return state === kPending;
  };

  while (fn())
    wait.run();

  if (state === kFulfilled)
    return result;
  else
    throw result;
}

wait.run = () => {
  process._tickDomainCallback();
  binding.run();
};

module.exports = wait;
