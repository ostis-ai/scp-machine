# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).


## [Unreleased]

### Added

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
