About structs in the API's header files:
	To reduce the amount of heap allocations, many Purple struct definitions are kept in
	header files. By doing so, encapsulation does get in part broken, but this measure is
	important for some possibly performance critic structures.

	Recognizing the above problems, it is settled that struct fields prefixed by an underscore
	are meant to be part of the struct private implementation. Do not access those fields directly,
	as access to them is not documented and not standardized, making their use susceptible to undefined
	behaviour. Fields not prefixed by underscores can always be safely viewed/edited.