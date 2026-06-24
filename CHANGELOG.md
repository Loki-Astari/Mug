# Changelog

All notable changes to this project will be documented in this file.

This component is part of [ThorsAnvil](https://github.com/Loki-Astari/ThorsAnvil). See the [parent changelog](https://github.com/Loki-Astari/ThorsAnvil/blob/master/CHANGELOG.md) for full release history.

The format is based on [Keep a Changelog](https://keepachangelog.com/).

## [10.3.0] - 2026-06-24

### Added
- Added signal handlers for graceful shutdown
- Improved restart/stop behavior

### Fixed
- Improved logging and fixed bug

## [10.0.0] - 2026-05-15

### Changed
- Changed Mug interface
- Simplified creation of `MugPlugin` for the simple case
- Updated to new version of ThorsSlack

## [9.2.0] - 2026-04-10

### Security
- Replaced `std::string::operator==` with `CRYPTO_memcmp` for secure comparison

### Added
- Header-only builds enabled

## [9.1.7] - 2026-03-22

### Changed
- Removed Slack from Mug
- Updated build tools
