# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Changed

- Use ninja generator in cmake
- Rename Conan remote repository url to https://conan.ostis.net/artifactory/api/conan/ostis-ai-library

### Fixed

- Move CONTRIBUTING.md to docs folder

## [0.1.0] -- 31.01.2025

### Added

- Developer tool to review PRs -- Ellipsis
- Intro for scp-machine in docs
- CMake flags section in docs
- Quick start section for users in docs
- Quick start section for developers in docs
- Documentation for all agents
- Sys gen and sys search operators
- Add README.md and info about build
- Add changelog
- Cherry-pick scp-interpreter from https://github.com/ostis-dev/scp-machine

### Changed

- Build scp-machine with conan-provided dependencies
- Install sc-machine with conan
- Rename questions to actions

### Fixed

- sc-memory API usage
- Remove system identifiers usage in parameters for generating program template

### Removed

- Remove codegen for agents
- CMake Windows dependencies
