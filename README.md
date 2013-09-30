systemd-kde-agent
=================

systemd-kde-agent is a [systemd password agent] for KDE. It asks for passwords, such as needed to 
mount an encrypted disk drive, on the desktop and sends them back to systemd using PolicyKit to 
acquire privileges (which means that you need to have a PolicyKit agent running).

License
-------
systemd-kde-agent is licensed under the GPLv2+, and includes code from polkit-kde-agent-1.

[systemd password agent]: http://www.freedesktop.org/wiki/Software/systemd/PasswordAgents
