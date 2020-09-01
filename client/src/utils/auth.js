function checkAuthenticated(response) {
  return !(response.status === 403 &&
      response.data["authenticated"] !== undefined &&
      !response.data["authenticated"]);
}

export { checkAuthenticated };
