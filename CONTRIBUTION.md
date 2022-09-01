# Contribution Guidelines

## Naming Scheme

|        Scope | Scheme               |                                                        |
| -----------: | :------------------- | :----------------------------------------------------- |
|        Files | PascalScheme         |                                                        |
|    Functions | PascalScheme         |                                                        |
|    Variables | camelCase            | suffix: typedef => `_t`, static => `_s`, const => `_c` |
| Preprocessor | SCREAMING_SNAKE_CASE |                                                        |

Functions and variables should be prefixed with a consistent identifier.

## Push

Don't push directly to the `main` branch. Push your modification to a new branch and open a push request to `main` for
these modifications.
