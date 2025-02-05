void
tilewide(Monitor *m)
{
        unsigned int i, n, w, h, mw, mx, ty;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	if (n > m->nmaster)
		mw = m->nmaster ? m->ww * m->mfact : 0;
	else
		mw = m->ww;
	for (i = mx = ty = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
		        w = (mw - mx) / (MIN(n, m->nmaster) - i);
		        resize(c, m->wx + mx, m->wy, w - (2*c->bw), (m->wh - ty) - (2*c->bw), 0);
		        if  (mx + WIDTH(c) < m->ww)
		                mx += WIDTH(c);
		} else {
			h = (m->wh - ty) / (n - i);
			resize(c, m->wx + mw, m->wy + ty, m->ww - mw - (2*c->bw), h - (2*c->bw), 0);
			if (ty + HEIGHT(c) < m->wh)
				ty += HEIGHT(c);
		}
}
