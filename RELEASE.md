# Release guide

This repo is set up for GitHub Releases.

## Local flow

1. Tag the release:

```sh
git tag v1.0.0
```

2. Push the branch and tag:

```sh
git push origin main
git push origin v1.0.0
```

3. Create the GitHub release:

```sh
gh release create v1.0.0 --title "v1.0.0" --notes-file CHANGELOG.md
```

## Notes

- No release assets are required for v1.0.0.
- The binary is built locally from source.
- The project is intended to be public under `chiragmehta/is-modifier-down`.
