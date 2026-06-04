# Tests

This document describes the unit-test additions and how they integrate with the GitHub Actions workflow.

## Modules used by the CI workflow

The repository includes a GitHub Actions workflow at `.github/workflows/tests.yml` that can run the test binary with selected modules. The workflow recognizes the following module keywords (use these in the `modules` input when manually dispatching the workflow):

- `octree`
- `quadtree`
- `halfedge` (maps to the Catch2 tag `[extendedHalfEdge]`)
- `mesh`
- `printableMesh` (maps to the Catch2 tag `[PrintableMesh]`)
- `ray` (maps to the Catch2 tag `[ray]`)
- `ray2D` (maps to the Catch2 tag `[ray2D]`)
- `aabb` (maps to the Catch2 tag `[aabb]`)

When the workflow is triggered by a pull request it enables the default set: `octree quadtree halfedge mesh printableMesh ray ray2D aabb`.

When triggering the workflow manually (`workflow_dispatch`) you can override `modules` (default value includes all modules listed above).

## How the workflow passes test filters

The workflow builds the project and assembles a `TEST_ARGS` string containing Catch2 tags derived from the selected modules. The test runner is invoked as:

```bash
./Test $TEST_ARGS
```

Examples:

- Run only octree tests:

```bash
./Test [octree],
```

- Run octree and ray tests:

```bash
./Test [octree],[ray],
```

## Notes

- Tests are written using Catch2 and are intentionally small and deterministic.
- If you change the workflow `modules` defaults or add new test tags, update this document accordingly.
