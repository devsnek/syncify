const binding = require('bindings')('uv_run_once');
const {
  getPromiseDetails,
  kPending,
  kFulfilled,
} = process.binding('util');

function syncify(promise) {
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
    syncify.uvRunOnce();

  if (state === kFulfilled)
    return result;
  else
    throw result;
}

syncify.uvRunOnce = () => {
  process._tickDomainCallback();
  binding.run();
};

module.exports = syncify;
