bool gcc3symCharPredicate(char c) {
	switch(c) {
		case '_':
		case '$':
			return true;
		default:
			return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
	}
}
